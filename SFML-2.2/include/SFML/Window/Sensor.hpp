et=" + f.format(new Date(mTodaySunset))
                    + ", mTomorrowSunrise=" + f.format(new Date(mTomorrowSunrise))
                    + "}";
        }
    }

    /**
     * Listener for changes in twilight state.
     */
    public interface TwilightListener {
        public void onTwilightStateChanged();
    }

    private static final class TwilightListenerRecord implements Runnable {
        private final TwilightListener mListener;
        private final Handler mHandler;

        public TwilightListenerRecord(TwilightListener listener, Handler handler) {
            mListener = listener;
            mHandler = handler;
        }

        public void post() {
            mHandler.post(this);
        }

        @Override
        public void run() {
            mListener.onTwilightStateChanged();
        }
    }

    private final class LocationHandler extends Handler {
        private static final int MSG_ENABLE_LOCATION_UPDATES = 1;
        private static final int MSG_GET_NEW_LOCATION_UPDATE = 2;
        private static final int MSG_PROCESS_NEW_LOCATION = 3;
        private static final int MSG_DO_TWILIGHT_UPDATE = 4;

        private static final long LOCATION_UPDATE_MS = 24 * DateUtils.HOUR_IN_MILLIS;
        private static final long MIN_LOCATION_UPDATE_MS = 30 * DateUtils.MINUTE_IN_MILLIS;
        private static final float LOCATION_UPDATE_DISTANCE_METER = 1000 * 20;
        private static final long LOCATION_UPDATE_ENABLE_INTERVAL_MIN = 5000;
        private static final long LOCATION_UPDATE_ENABLE_INTERVAL_MAX =
                15 * DateUtils.MINUTE_IN_MILLIS;
        private static final double FACTOR_GMT_OFFSET_LONGITUDE =
                1000.0 * 360.0 / DateUtils.DAY_IN_MILLIS;

        private boolean mPassiveListenerEnabled;
        private boolean mNetworkListenerEnabled;
        private boolean mDidFirstInit;
        private long mLastNetworkRegisterTime = -MIN_LOCATION_UPDATE_MS;
        private long mLastUpdateInterval;
        private Location mLocation;
        private final TwilightCalculator mTwilightCalculator = new TwilightCalculator();

        public void processNewLocation(Location location) {
            Message msg = obtainMessage(MSG_PROCESS_NEW_LOCATION, location);
            sendMessage(msg);
        }

        public void enableLocationUpdates() {
            sendEmptyMessage(MSG_ENABLE_LOCATION_UPDATES);
        }

        public void requestLocationUpdate() {
            sendEmptyMessage(MSG_GET_NEW_LOCATION_UPDATE);
        }

        public void requestTwilightUpdate() {
            sendEmptyMessage(MSG_DO_TWILIGHT_UPDATE);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_PROCESS_NEW_LOCATION: {
                    final Location location = (Location)msg.obj;
                    final boolean hasMoved = hasMoved(mLocation, location);
                    final boolean hasBetterAccuracy = mLocation == null
                            || location.getAccuracy() < mLocation.getAccuracy();
                    if (DEBUG) {
                        Slog.d(TAG, "Processing new location: " + location
                               + ", hasMoved=" + hasMoved
                               + ", hasBetterAccuracy=" + hasBetterAccuracy);
                    }
                    if (hasMoved || hasBetterAccuracy) {
                        setLocation(location);
                    }
                    break;
                }

                case MSG_GET_NEW_LOCATION_UPDATE:
                    if (!mNetworkListenerEnabled) {
                        // Don't do anything -- we are still trying to get a
                        // location.
                        return;
                    }
                    if ((mLastNetworkRegisterTime + MIN_LOCATION_UPDATE_MS) >=
                            SystemClock.elapsedRealtime()) {
                        // Don't do anything -- it hasn't been long enough
                        // since we last requested an update.
                        return;
                    }

                    // Unregister the current location monitor, so we can
                    // register a new one for it to get an immediate update.
                    mNetworkListenerEnabled = false;
                    mLocationManager.removeUpdates(mEmptyLocationListener);

                    // Fall through to re-register listener.
                case MSG_ENABLE_LOCATION_UPDATES:
                    // enable network provider to receive at least location updates for a given
                    // distance.
                    boolean networkLocationEnabled;
                    try {
                        networkLocationEnabled =
                            mLocationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
                    } catch (Exception e) {
                        // we may get IllegalArgumentException if network location provider
                        // does not exist or is not yet installed.
                        networkLocationEnabled = false;
                    }
                    if (!mNetworkListenerEnabled && networkLocationEnabled) {
                        mNetworkListenerEnabled = true;
                        mLastNetworkRegisterTime = SystemClock.elapsedRealtime();
                        mLocationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,
                                LOCATION_UPDATE_MS, 0, mEmptyLocationListener);

                        if (!mDidFirstInit) {
                            mDidFirstInit = true;
                            if (mLocation == null) {
                                retrieveLocation();
                            }
                        }
                    }

   