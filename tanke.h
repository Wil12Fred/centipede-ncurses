using namespace std;

typedef pair <int, int > pii;

class tanke{
  public:
    int y;
    int* Z;
    void Crear (int z,int* Z){
      this->Z=Z;
      y=z/2;
      return;
    }
    int coor(int z){ // z tamaño de la matriz
      if(z%2!=0) return (z-1)*z+y;
      return (z-1)*z+(z-y-1);
    }
    void actualizar(int z,int dir){
      //*(Z+y+dir)=1;
      //mvaddch(0,y,' ');
      y+=dir;
      if (y<0) y=0;
      if (y>z-1) y=z-1;
      mvaddch(z-0,y,'a');
      return;
    }
    int Get(){
      return y;
    }
};
