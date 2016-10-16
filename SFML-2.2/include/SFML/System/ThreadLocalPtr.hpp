ion == connection) {
            if (DEBUG) {
                Slog.d(TAG, this + ": Service connection died");
            }
            disconnect();
        }
    }

    private void onDisplayStateChanged(Connection connection, RemoteDisplayState state) {
        if (mActiveConnection == connection) {
            if (DEBUG) {
                Slog.d(TAG, this + ": State changed, state=" + state);
            }
            setDisplayState(state);
        }
    }

    private void disconnect() {
        if (mActiveConnection != null) {
            if (mSelectedDisplayId != null) {
                mActiveConnection.disconnect(mSelectedDisplayId);
            }
            mConnectionReady = false;
            mActiveConnection.dispose();
            mActiveConnection = null;
            setDisplayState(null);
        }
    }

    private void setDisplayState(RemoteDisplayState state) {
        if (!Objects.equal(mDisplayState, state)) {
            mDisplayState = state;
            if (!mScheduledDisplayStateChangedCallback) {
                mScheduledDisplayStateChangedCallback = true;
                mHandler.post(mDisplayStateChanged);
            }
        }
    }

    @Override
    public String toString() {
        return "Service connection " + mComponentName.flattenToShortString();
    }

    private final Runnable mDisplayStateChanged = new Runnable() {
        @Override
        public void run() {
            mScheduledDisplayStateChangedCallback = false;
            if (mDisplayStateCallback != null) {
                mDisplayStateCallback.onDisplayStateChanged(
                        RemoteDisplayProviderProxy.this, mDisplayState);
            }
        }
    };

    public interface Callback {
        void onDisplayStateChanged(RemoteDisplayProviderProxy provider, RemoteDisplayState state);
    }

    private final class Connection implements DeathRecipient {
        private final IRemoteDisplayProvider mProvider;
        private final ProviderCallback mCallback;

        public Connection(IRemoteDisplayProvider provider) {
            mProvider = provider;
            mCallback = new ProviderCallback(this);
        }

        public boolean register() {
            try {
                mProvider.asBinder().linkToDeath(this, 0);
                mProvider.setCallback(mCallback);
                mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        onConnectionReady(Connection.this);
                    }
                });
                return true;
            } catch (RemoteException ex) {
                binderDied();
            }
            return false;
        }

        public void dispose() {
            mProvider.asBinder().unlinkToDeath(this, 0);
            mCallback.dispose();
        }

        public void setDiscoveryMode(int mode) {
            try {
                mProvider.setDiscoveryMode(mode);
            } catch (RemoteException ex) {
                Slog.e(TAG, "Failed to deliver request to set discovery mode.", ex);
            }
        }

        public void connect(String id) {
            try {
                mProvider.connect(id);
            } catch (RemoteException ex) {
                Slog.e(TAG, "Failed to deliver request to connect to display.", ex);
            }
        }

        public void disconnect(String id) {
            try {
                mProvider.disconnect(id);
            } catch (RemoteException ex) {
                Slog.e(TAG, "Failed to deliver request to disconnect from display.", ex);
            }
        }

        public void setVolume(String id, int volume) {
            try {
                mProvider.setVolume(id, volume);
            } catch (RemoteException ex) {
                Slog.e(TAG, "Failed to deliver request to set display volume.", ex);
            }
        }

        public void adjustVolume(String id, int volume) {
            try {
                mProvider.adjustVolume(id, volume);
            } catch (RemoteException ex) {
                Slog.e(TAG, "Failed to deliver request to adjust display volume.", ex);
            }
        }

        @Override
        public void binderDied() {
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    onConnectionDied(Connection.this);
                }
            });
        }

        void postStateChanged(final RemoteDisplayState state) {
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    onDisplayStateChanged(Connection.this, state);
                }
            });
        }
    }

    /**
     * Receives callbacks from the service.
     * <p>
     * This inner class is static and only retains a weak reference to the connection
     * to prevent the client from being leaked in case the service is holding an
     * active reference to the client's callb