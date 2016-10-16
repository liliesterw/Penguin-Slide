#include <SFML/Graphics.hpp>
#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<vector>
using namespace std;
using namespace sf;
double tileWidth =50.0;
double tileHeight =50.0;
int timees =0;
int offset2 = tileWidth/2;
int offset = 50;
int maxscore=80;
int score=0;
int minscore = 10;
int key=1;
Time myTimeSprite;
Clock myClockSprite;
Time myTime;
Clock myClock;
//int rotasi = 0;
void displayPosition(Vector2f temp)
{
	//cout<<temp.x<<" "<<temp.y<<endl;
}
class IndexArah
{
public:
	int baris, kolom,arah;
	IndexArah(int _baris=0,int _kolom=0,int _arah=0)
	{
		baris = _baris;
		kolom  =_kolom;
		arah = _arah;
		//cout<<baris<<" "<<kolom<<" "<<arah<<endl;
	}
	/*
	IndexArah operator<<(const IndexArah temp)
	{
	cout<<temp.baris<<" "<<temp.kolom<<" "<<temp.arah<<endl;
	}*/
};
class Index
{
public:
	int _baris,_kolom;
	Index(int baris=0, int kolom=0)
	{
		_baris = baris;
		_kolom=kolom;
		//cout<<_baris<<","<<_kolom<<endl;
	}
	void setBK(int b,int k)
	{
		_baris=b;
		_kolom=k;
	}
	void display()
	{
		cout<<_baris<<" "<<_kolom<<endl;
	}
};
class Grid
{
private:
	int _w, _h,_c;
	int counter;//class grid tapi g simpen array 2d?pindah nde global..wkwkkwk.. soale pas ngeprint e bingung pisan..
	int _grid[10][10];
	int _ans[10][10];
	int moves;
	int x;
	int y;

	int _startY,_startX,_direction;
	Sprite s_pembelok[4],s_batu,s_goal,s_start,s_coin,s_lurus,s_menu,s_score,
		s_time,s_back,s_go,s_quit,s_walk,s_right,s_success,s_over,s_menus,s_play;

	vector<IndexArah> indexPath;
public:
	Grid()
	{
		_h = 0;
		_w = 0;
		_c =0;
		counter =0;
		moves=0;
		y=0;
	}
	Sprite getscoin()
	{
		return s_coin;
	}
	Sprite getgo()
	{
		return s_go;
	}
	Sprite getplay()
	{
		return s_play;
	}
	Sprite getquit()
	{
		return s_quit;
	}
	int getGrid(int _baris,int _kolom)
	{
		return _grid[_baris][_kolom];
	}
	void setGrid(FILE *&pf)
	{
		moves =0 ;
		fscanf_s(pf,"%d %d %d",&_h,&_w,&_c);
		cout<<"Kode " <<_c<<endl;
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
			{
				fscanf_s(pf, "%d", &_grid[i][j]);
				_ans[i][j] = _grid[i][j];

				if(_grid[i][j] == 2) {_startX=i-1 ; _startY=j;_direction=0;}
				else if(_grid[i][j] == 3) {_startX=i ; _startY=j+1;_direction=1;}
				else if(_grid[i][j] == 4) {_startX=i+1 ; _startY=j;_direction=2;}
				else if(_grid[i][j] == 5) {_startX=i ; _startY=j-1;_direction=3;}


				if(_grid[i][j]==-2 ||_grid[i][j]==-3||_grid[i][j]==-4||_grid[i][j]==-5)
				{

					_grid[i][j] = ((rand() % 3) +2 )*-1;


					if(-1*_grid[i][j] <= _ans[i][j]*-1)
					{
						//cout<<_grid[i][j]<< "-"<<_ans[i][j];
						moves = moves+ ((-1*_grid[i][j]) - (-1*_ans[i][j]));
						//cout<<"moves : "<< i<<","<<j<<":"<<(-1*_grid[i][j]) -( -1*_ans[i][j])<<endl;
					}
					else {moves=moves + (-1*(_grid[i][j])- -1*(_ans[i][j]-4));
					//cout<<"moves : "<< i<<","<<j<<":"<<-1*(_grid[i][j])- (-1*(_ans[i][j]-4))<<endl;
					}
				}
				if(_grid[i][j]==-6||_grid[i][j]==-7)
				{
					_grid[i][j] = ((rand() % 2)+6 )*-1;
					//cout<<_grid[i][j];
					if(_grid[i][j] != _ans[i][j]){moves--;}
				}
				//cout<<moves<<endl;
				if(_grid[i][j] == -8)
				{
					_grid[i][j] = ((rand() % 3) +2 )*-1;
				}
				if(_grid[i][j]==-9)
				{
					_grid[i][j] = ((rand() % 2)+6 )*-1;
				}

			}

		}

		moves= moves*-1;

		//cout<<"moves : "<<moves<<endl;
		//cout<<"Score : "<<score<<endl;
	}
	int getW()
	{
		return _w;
	}
	int getH()
	{
		return _h;
	}
	int getMoves()
	{
		moves --;

		return moves;
	}
	int getMove()
	{
		return moves;
	}
	int getScore(int x)
	{
		if(score<=minscore)
		{score -=0;}
		else {
		score -= 5;
		}
		return score;
	}
	void changeGrid(int i,int j)
	{
		if(_grid[i][j] == -2) _grid[i][j]= -3;
		else if(_grid[i][j] == -3) _grid[i][j]= -4;
		else if(_grid[i][j] == -4) _grid[i][j]= -5;
		else if(_grid[i][j] == -5) _grid[i][j]= -2;
		else if(_grid[i][j] == -6)_grid[i][j] = -7;
		else if(_grid[i][j] == -7)_grid[i][j] = -6;
	}
	void setTextures(const Texture &t_pembelok, const Texture &t_batu, const Texture &t_goal, const Texture &t_start, const Texture &t_coin,const Texture &t_lurus,const Texture &t_menu,const Texture &t_score,const Texture &t_time,const Texture &t_back,const Texture &t_go,const Texture &t_quit,const Texture &t_walk,const Texture &t_right,const Texture &t_success,const Texture &t_over,const Texture &t_menus,const Texture &t_play)
	{
		s_batu.setTexture(t_batu);
		//cara buat ukuran gambar 32x32
		s_batu.setScale(tileWidth/227,tileHeight/222);
		//sprite laine juga sama
		s_goal.setTexture(t_goal);

		s_score.setTexture(t_score);
		s_score.setScale(0.3,0.3);

		s_goal.setScale(tileWidth/102,tileHeight/102);
		s_start.setTexture(t_start);
		s_start.setScale(tileWidth/124,tileHeight/124);
		s_coin.setTexture(t_coin);
		s_coin.setScale(tileWidth/102,tileHeight/102);
		s_pembelok[0].setTexture(t_pembelok);
		s_pembelok[0].setScale(tileWidth/124,tileHeight/124);
		s_lurus.setTexture(t_lurus);
		s_lurus.setScale(tileWidth/124,tileHeight/124);
		s_menu.setTexture(t_menu);

		s_go.setTexture(t_go);
		s_go.setScale(0.5,0.5);
		s_quit.setTexture(t_quit);
		s_quit.setScale(0.5,0.5);
		s_time.setTexture(t_time);

		s_back.setTexture(t_back);

		s_walk.setTexture(t_walk);
		s_walk.setScale(1.3,1.3);
		s_right.setTexture(t_right);
		s_success.setTexture(t_success);
		s_over.setTexture(t_over);
		s_menus.setTexture(t_menus);
		s_play.setTexture(t_play);
	}
	int getStartY()
	{
		return _startY;
	}
	int getStartDirection()
	{
		return _direction;
	}
	void displayGrid()
	{
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
				cout << _grid[i][j] << " ";
			cout << endl;
		}
	}

	bool isValid(int b, int k)
	{
		if (b < 0 || k < 0 || b >= _h || k >= _w || _grid[b][k] == -1)
			return false;
		return true;
	}
	void swapTile(int row1, int col1, int row2,int col2)
	{
		int temp = _grid[row1][col1];
		_grid[row1][col1]=_grid[row2][col2];
		_grid[row2][col2]=temp;
	}
	bool doTraceLight()
	{

		return traceLight(_startX,_startY,_direction,0);
		//tak kasih hint
		//setiap kali pindah kotak disimpen koord e
		//terus di return ke game
		//setiap 0,5 detik posisi robot di set koord e
		// contoh misal e posisi[5]; y brarti setiap 0,5 detik nanti bakal pindah dari posisi[0], posisi[1], posisi[2] dst return ke game mksde?
		//berarti aku pny 1 array kan? xg menyimpan robot ini lewat mana ae, btw ini km tak ajari vector ae supaya km lebih gmpg bikin apa ae
		//tak return ke game supaya game tau robot harus lwat mana ae h
	}
	int traceLight(int b,int k, int lightDirection, int _stepToGoal)
	{
		indexPath.push_back(IndexArah(b,k,lightDirection));
		//cout<<indexPath.size()<<endl;
		//inti e gini setiap kali aku nerusno sinar ke kotak laine step ku bertambah
		//kan kapan hari km ws simpen to jumlah kotak kosong xg dilewatin mb sinar e? uda? uda itu pembelok km itung sebagi step ga? nda
		if (!isValid(b, k))
			return 0;
		if(_grid[b][k]==9) {cout<<_stepToGoal<<endl;return _stepToGoal;}
		if (_grid[b][k] == 0)
		{
			return 0;
		}
		if (_grid[b][k] == -6 || _grid[b][k] == -7)
		{
			//_grid[b][k] = 11;
			int te=0;

			if (lightDirection == 0 && _grid[b][k] == -7) te = traceLight(b - 1, k, 0,_stepToGoal+1);//Meneruskan sinar ke atas
			if (lightDirection == 1&& _grid[b][k] == -6) te = traceLight(b, k + 1, 1,_stepToGoal+1);//Meneruskan sinar ke kanan
			if (lightDirection == 2 && _grid[b][k] == -7) te = traceLight(b + 1, k, 2,_stepToGoal+1);//Meneruskan sinar ke bawah
			if (lightDirection == 3&& _grid[b][k] == -6) te = traceLight(b, k-1, 3,_stepToGoal+1);//Meneruskan sinar ke kiri
			//cout<<_stepToGoal<<te<<endl;
			if (te> 0){return te;}
		}//kalo mw semakin banyak pembelok e tambahi ae else if
		/*else if (_grid[b][k] == 100)
		{
		//dee ngapain gitu tok to? haha cobaa kalo 100 denerusno ke >awal e dari mana? bawah atas kiri kanan? dari kiri ka kanan
		//parameter ke 3 function e itu kan nyimpenarah
		//if(lightDirection==kanan) traceLight(b,k+1,kanan,_stepToGoal)//ke kanan berarti b tetep tapi k+1, beres :D okey2 return? temp?percuma se pasti 0 nti hasil e soal e kan dee g bertambah sama sekali alias lek kenal 0 lgsg di return 0 jadi g diterusno klo 0, btw hruse gini wes jalan
		}*/


		else if (_grid[b][k] == -2 && lightDirection == 3) //ditambahi gini bukan? iya tapi aku gtw lho itu lightdirection mu ws bener atau g,liaten nd catetan mu..harus e wes bener
		{
			int temp = traceLight(b - 1, k, 0,_stepToGoal);//Membelokkan sinar ke atas 1
			if(temp>0) return temp;
		}
		else if (_grid[b][k] == -5 && lightDirection == 1)
		{
			int temp = traceLight(b - 1, k, 0,_stepToGoal);//Membelokkan sinar ke atas 2
			if(temp>0) return temp;
		}
		else if (_grid[b][k] == -3  && lightDirection ==0) 
		{
			int temp =traceLight(b, k + 1, 1,_stepToGoal);//Membelokkan sinar ke kanan 1
			if(temp>0) return temp;
		}
		else if (_grid[b][k] == -2  && lightDirection ==2) 
		{
			int temp =traceLight(b, k + 1, 1,_stepToGoal);//Membelokkan sinar ke kanan 2
			if(temp>0) return temp;
		}
		else if (_grid[b][k] == -4  && lightDirection ==1) 
		{
			int temp = traceLight(b + 1, k, 2,_stepToGoal);//Membelokkan sinar ke bawah 1
			if(temp>0) return temp;
		} 
		else if (_grid[b][k] == -3  && lightDirection ==3) 
		{
			int temp = traceLight(b + 1, k, 2,_stepToGoal);//Membelokkan sinar ke bawah 2
			if(temp>0) return temp;
		} 
		else if (_grid[b][k] == -5 && lightDirection ==2) 
		{
			int temp =traceLight(b, k - 1, 3,_stepToGoal);//Membelokkan sinar ke kiri 1
			if(temp>0) return temp;
		}
		else if (_grid[b][k] == -4 && lightDirection ==0) 
		{
			int temp =traceLight(b, k - 1, 3,_stepToGoal);//Membelokkan sinar ke kiri 2
			if(temp>0) return temp;
		}
		return 0;
	}
	void draw( RenderWindow &window) 
	{

		if(key==2 || key==5)
		{
			if(score<0)s_score.setColor(Color::Red);
			int yy = abs(score);
			int ye = 0;
			//window.draw(myscore);

			window.draw(s_back);

			while( yy != 0 ){
				int sat = yy % 10;
				yy=yy/10;

				s_score.setTextureRect(IntRect((sat)*90,0,90,90));
				s_score.setPosition(467-(ye*25),14);
				s_score.setColor(Color::White);
				window.draw(s_score);
				ye ++ ;

				s_time.setPosition(42,14);

				s_time.setTextureRect(IntRect(0,0,262-(0.44)*(myClock.getElapsedTime().asSeconds()),22));
				if(262-(0.44)*(myClock.getElapsedTime().asSeconds()) <0)
				{
					key = 9;
				}
				s_quit.setPosition(635,520);
				s_go.setPosition(415,520);
				window.draw(s_quit);
				window.draw(s_go);
				window.draw(s_time);
			}
			for(int i=0;i<_h;i++)
			{

				for(int j=0;j<_w;j++)
				{
					//window.draw(s_pembelok[0]);

					if(_grid[i][j]==8)//kode e batu
					{
						s_batu.setPosition(offset+j*tileWidth ,offset+i*tileHeight);
						window.draw(s_batu);
					}
					else if(_grid[i][j]==9)//kode goal
					{
						s_goal.setPosition(offset+j*tileWidth ,offset+i*tileHeight);
						window.draw(s_goal);
					}
					else if(_grid[i][j]==-2)//kode pembelok atas
					{
						s_pembelok[0].setOrigin(0,0);
						s_pembelok[0].setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_pembelok[0].setOrigin(62,62);
						s_pembelok[0].setRotation(270);
						window.draw(s_pembelok[0]);
						s_pembelok[0].setOrigin(0,0);
					}
					else if(_grid[i][j]==-3)//kode pembelok kanan
					{
						s_pembelok[0].setOrigin(0,0);
						s_pembelok[0].setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_pembelok[0].setOrigin(62,62);
						s_pembelok[0].setRotation(0);
						window.draw(s_pembelok[0]);
						s_pembelok[0].setOrigin(0,0);
					}
					else if(_grid[i][j]==-4)//kode pembelok bawah
					{
						s_pembelok[0].setOrigin(0,0);
						s_pembelok[0].setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_pembelok[0].setOrigin(62,62);
						s_pembelok[0].setRotation(90);
						window.draw(s_pembelok[0]);
						s_pembelok[0].setOrigin(0,0);
					}
					else if(_grid[i][j]==-5)//kode pembelok kiri
					{
						s_pembelok[0].setOrigin(0,0);
						s_pembelok[0].setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_pembelok[0].setOrigin(62,62);
						s_pembelok[0].setRotation(180);
						window.draw(s_pembelok[0]);
						s_pembelok[0].setOrigin(0,0);
					}
					else if(_grid[i][j] == -6)
					{
						s_lurus.setOrigin(0,0);
						s_lurus.setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_lurus.setOrigin(62,62);
						s_lurus.setRotation(90);
						window.draw(s_lurus);
						s_lurus.setOrigin(0,0);
					}
					else if(_grid[i][j] == -7)
					{
						s_lurus.setOrigin(0,0);
						s_lurus.setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_lurus.setOrigin(62,62);
						s_lurus.setRotation(0);
						window.draw(s_lurus);
						s_lurus.setOrigin(0,0);
					}
					//---------------------------------------------------------------------------------------------------------
					else if(_grid[i][j]==2)//kode start atas
					{
						s_start.setOrigin(0,0);
						s_start.setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_start.setOrigin(62,62);
						s_start.setRotation(270);
						window.draw(s_start);
						s_start.setOrigin(0,0);
					}
					else if(_grid[i][j]==3)//kode pembelok kanan
					{
						s_start.setOrigin(0,0);
						s_start.setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_start.setOrigin(62,62);
						s_start.setRotation(0);
						window.draw(s_start);
						s_pembelok[0].setOrigin(0,0);
					}
					else if(_grid[i][j]==4)//kode pembelok bawah
					{
						s_start.setOrigin(0,0);
						s_start.setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_start.setOrigin(62,62);
						s_start.setRotation(90);
						window.draw(s_start);
						s_start.setOrigin(0,0);
					}
					else if(_grid[i][j]==5)//kode pembelok kiri
					{
						s_start.setOrigin(0,0);
						s_start.setPosition(offset+offset2+j*tileWidth ,offset+offset2+i*tileHeight);
						s_start.setOrigin(62,62);
						s_start.setRotation(180);
						window.draw(s_start);
						s_start.setOrigin(0,0);
					}
					else if(_grid[i][j]==0)
					{
						s_coin.setPosition(offset+j*tileWidth,offset+i*tileHeight );
						window.draw(s_coin);

					}
				}


			}
		}
		if(key==1)
		{
			window.draw(s_menus);

			s_play.setPosition(500,500);
			//window.draw(s_menu);
			window.draw(s_play);

		}
		if(key==9)
		{

			int ye = 0;
			int yy = abs(score);
			window.draw(s_over);
			while( yy != 0 ){
				int sat = yy % 10;
				yy=yy/10;

				s_score.setTextureRect(IntRect((sat)*90,0,90,90));
				s_score.setPosition(700-(ye*25),540);
				s_score.setColor(Color::White);
				window.draw(s_score);
				ye ++ ;
			}

		}
		if(key==5)
		{
			//cout<<"size: "<<indexPath.size()<<endl;
			//cout<<indexPath[0].baris;
			//penguin jalan start
			/*
			if(myClockSprite.getElapsedTime().asSeconds() > 0.5)
			{
			if(timees== 2) x= -1;
			else if(timees ==0) x = 1;
			timees = timees + x ;
			//cout<<timees<<endl;
			s_walk.setPosition(offset+0*tileWidth +5,offset+y+0*tileHeight);
			//cout<<offset+0*tileWidth +5<<" "<<offset+y+0*tileHeight<<endl;
			y+=5;

			myClockSprite.restart();
			}

			s_walk.setTextureRect(IntRect(timees*30,0,23,30));
			window.draw(s_walk);
			*/
			//penguin jalan finish


			if(-1200 + 800*myClockSprite.getElapsedTime().asSeconds() <0)
			{
				s_success.setPosition((-1200 + 800*myClockSprite.getElapsedTime().asSeconds()),0);
			}
			else{
				//cout<<myClockSprite.getElapsedTime().asSeconds()<<endl;
				s_success.setPosition(0,0);
			}
			if(myClockSprite.getElapsedTime().asSeconds() > 3.6)
			{ 
				key = 6;
			}
			

			window.draw(s_success);
			if(myClockSprite.getElapsedTime().asSeconds() > 3.5 &&myClockSprite.getElapsedTime().asSeconds() < 3.7)
			{ 
				window.draw(s_back);
				//key=6;
			}
			//window.draw(s_right);

		}

	}
};
class Game
{
private:
	Grid _soal[15];
	int _currentSoal;

	RenderWindow window;
	Texture t_pembelok, t_batu, t_goal,t_start,t_coin,t_lurus,t_menu,t_score,t_time,t_back,t_go,t_quit,t_walk,t_right,t_success,t_over,t_menus,t_play;	
	FILE *pf;

public:
	Game()
	{

		loadGame();//ini supaya load texture cm 1x tapi di pakai berkali kali

	}
	void loadGame()
	{
		window.create(VideoMode(800,600),"IRGL 2015 : Penguin Slide");
		fopen_s(&pf,"source.txt","r");
		t_pembelok.loadFromFile("pembelokES.png");
		t_goal.loadFromFile("finish.png");
		t_batu.loadFromFile("trees.png");
		t_start.loadFromFile("start.png");
		t_coin.loadFromFile("trees.png");
		t_lurus.loadFromFile("penerusES.png");
		//t_menu.loadFromFile("menu.jpg");
		t_score.loadFromFile("score.png");
		t_time.loadFromFile("time.png");
		t_back.loadFromFile("backfull.png");
		t_go.loadFromFile("go.png");
		t_quit.loadFromFile("quit.png");
		t_walk.loadFromFile("sprite1.png");
		t_right.loadFromFile("right.png");
		t_success.loadFromFile("success.png");
		t_over.loadFromFile("over.png");
		t_menus.loadFromFile("menu.png");
		t_play.loadFromFile("play.png");
		//_soal[0].displayGrid();
		for(int i=0;i<15;i++)
		{
			_soal[i].setGrid(pf);
			_soal[i].setTextures(t_pembelok,t_batu,t_goal,t_start,t_coin,t_lurus,t_menu,t_score,t_time,t_back,t_go,t_quit,t_walk,t_right,t_success,t_over,t_menus,t_play);
		}
		_currentSoal=0;
		score += maxscore + _soal[_currentSoal].getMove()*5;
		//load textures 
	}
	void test()
	{
		_soal[0].doTraceLight();
	}
	void play()
	{

		Index selectedIndex[2];
		selectedIndex[0].setBK(-1,-1);
		selectedIndex[1].setBK(-1,-1);
		bool mouseIsBeingPressed=false;
		Vector2f MousePos;
		Index selectedGrid[2];
		int gridPickCounter=0;
		bool result=false;
		while (window.isOpen())
		{
			sf::Event event;
			if(key==6)
			{
				key = 2;
				_currentSoal++;
				minscore= score;
				score += maxscore + _soal[_currentSoal].getMove()*5;
			}
			while (window.pollEvent(event))
			{

				//cout<<myClock.getElapsedTime().asSeconds()<<endl;
				if (event.type == sf::Event::Closed)
				{

					window.close();
				}
				if(Mouse::isButtonPressed(Mouse::Left))
				{ 
					if(key==9)
					{
						window.close();
					}
					if(key==1)
					{
						if(!mouseIsBeingPressed)
						{
							sf::Vector2f MousePos(sf::Mouse::getPosition(window));
							FloatRect pos=_soal[_currentSoal].getplay().getGlobalBounds();
							if(pos.contains(MousePos))
							{
								if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
								{
									cout<<"Start!\n";
									key++;
									myClock.restart() ;
								}
							}
						}
					}
					if(key==2){
						if(!mouseIsBeingPressed)
						{
							MousePos=Vector2f(Mouse::getPosition(window));

							selectedGrid[gridPickCounter]=Index((MousePos.y-offset)/tileHeight,(MousePos.x-offset)/tileHeight);
							cout<<MousePos.y<<" " <<MousePos.x<<endl;
							cout<<int((MousePos.y-offset)/tileHeight)<<" "<<int((MousePos.x-offset)/tileHeight)<<endl;
							//cout<<MousePos.x<<" , "<<MousePos.y<<endl;
							//cout<<"Baris:"<<selectedGrid[gridPickCounter - 1]._baris<<endl;
							//cout<<"Kolom:"<<selectedGrid[gridPickCounter - 1]._kolom<<endl;
							//cout<<_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter-1]._baris,selectedGrid[gridPickCounter-1]._kolom);
							if(MousePos.x<50 || MousePos.y<50 || MousePos.x>_soal[_currentSoal].getH()*tileWidth + offset ||MousePos.y>_soal[_currentSoal].getW()*tileWidth + offset)
							cout<<"OUT OF BORDER"<<endl;
							else {_soal[_currentSoal].changeGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom);
							if(selectedGrid[gridPickCounter]._baris >= _soal[_currentSoal].getH() || selectedGrid[gridPickCounter]._kolom >= _soal[_currentSoal].getW()||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==8 ||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==9 ||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==2 ||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==3||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==4||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==5||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==0){;}
							else { //cout<<_soal[_currentSoal].getMoves()<<endl;
							cout<<"Score : "<<_soal[_currentSoal].getScore(_currentSoal)<<endl;}
							}
							if(_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==8 ||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==9 ||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==2 ||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==3||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==4||_soal[_currentSoal].getGrid(selectedGrid[gridPickCounter]._baris,selectedGrid[gridPickCounter]._kolom)==5 )
							{
								cout<<"STUCK"<<endl;
								//_soal[_currentSoal].swapTile(selectedGrid[gridPickCounter-1]._baris,selectedGrid[gridPickCounter-1]._kolom,selectedGrid[gridPickCounter-1]._baris,selectedGrid[gridPickCounter-1]._kolom);
							}
						}
						mouseIsBeingPressed=true;
					
					FloatRect submit=_soal[_currentSoal].getgo().getGlobalBounds();
					FloatRect quit=_soal[_currentSoal].getquit().getGlobalBounds();
					if(quit.contains(MousePos))
					{
						if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
							window.close();
						}
					}
					if(submit.contains(MousePos))
					{
						if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){

							if(key!=5){result=_soal[_currentSoal].doTraceLight();}//result ngesimpen hasl dari trace light klo result true berarti robot e bisa sampe ke goal klo false g bisa

							if(result == true && key !=5)
							{
								key = 5;
								myClockSprite.restart();
								//_currentSoal ++;

							}
							else
							{ 
								_currentSoal++;
								_currentSoal--;
							}

						}
					}
					}
				}
				if(!Mouse::isButtonPressed(Mouse::Left))
					mouseIsBeingPressed=false;
			}


			window.clear(Color::Black);

			_soal[_currentSoal].draw(window);

			window.display();
		}

	}
	~Game()
	{
		_fcloseall();
	}

};
int main()
{
	srand(time(NULL));//coba en harus ws isa
	Game myGame;
	myGame.play();
selesai : 
	return 0;


}