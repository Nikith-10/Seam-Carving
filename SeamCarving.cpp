#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
using namespace cv;
using namespace std;
float calcEnergypix(int ***rgbmat,int x,int y,int rows,int cols)
{
    int w=cols;
    int h=rows;
    float Rx,Gx,Bx;
    //it is not a border pixel
    if(x!=0 && x!=h-1)
    {
        
        Rx=rgbmat[x+1][y][2]-rgbmat[x-1][y][2];
        Gx=rgbmat[x+1][y][1]-rgbmat[x-1][y][1];
        Bx=rgbmat[x+1][y][0]-rgbmat[x-1][y][0];
    }
    else if(x==0)
    {
        Rx=rgbmat[x+1][y][2]-rgbmat[h-1][y][2];
        Gx=rgbmat[x+1][y][1]-rgbmat[h-1][y][1];
        Bx=rgbmat[x+1][y][0]-rgbmat[h-1][y][0];
    }
    else if(x==h-1)
    {
        Rx=rgbmat[0][y][2]-rgbmat[x-1][y][2];
        Gx=rgbmat[0][y][1]-rgbmat[x-1][y][1];
        Bx=rgbmat[0][y][0]-rgbmat[x-1][y][0]; 
    }
    float Ry,Gy,By;
    if(y!=0 && y!=w-1)
    {
        // cout<<x<<" "<<y<<endl;
      Ry=rgbmat[x][y+1][2]-rgbmat[x][y-1][2];
      Gy=rgbmat[x][y+1][1]-rgbmat[x][y-1][1];
      By=rgbmat[x][y+1][0]-rgbmat[x][y-1][0];
    }
    else if(y==0)
    {
        // cout<<x<<" "<<y<<endl;
        Ry=rgbmat[x][y+1][2]-rgbmat[x][w-1][2];
        Gy=rgbmat[x][y+1][1]-rgbmat[x][w-1][1];
        By=rgbmat[x][y+1][0]-rgbmat[x][w-1][0];

    }
    else if(y==w-1)
    {
        // cout<<x<<" "<<y<<endl;
        Ry=rgbmat[x][0][2]-rgbmat[x][y-1][2];
        Gy=rgbmat[x][0][1]-rgbmat[x][y-1][1];
        By=rgbmat[x][0][0]-rgbmat[x][y-1][0];
    }
    float deltasqx=Rx*Rx+Gx*Gx+Bx*Bx;
    float deltasqy=Ry*Ry+Gy*Gy+By*By;
    float energy=sqrt(deltasqx+deltasqy);
    
    return energy;
}
//this returns an array consisting of column numbers to be removed
int *findverticalseam(float **energymatrix,int rows,int cols)
{
   
    int *vseam=new int[rows];
    float **costm;
    costm=new float *[rows];
    for(int i=0;i<rows;i++)
    {
        costm[i]=new float[cols];
    }
     
    //for first row it is same
    for(int i=0;i<cols;i++)
    {
        costm[0][i]=energymatrix[0][i];
    }
    //from second row we need to findout the cost
    for(int i=1;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            if(j==0)
            {
                costm[i][j]=energymatrix[i][j]+min(costm[i-1][j],costm[i-1][j+1]);
            }
            else if(j==cols-1)
            {
                costm[i][j]=energymatrix[i][j]+min(costm[i-1][j-1],costm[i-1][j]);
            }
            else{
                costm[i][j]=energymatrix[i][j]+min(costm[i-1][j-1],min(costm[i-1][j],costm[i-1][j+1]));
            }
        }
    }
     float mini=costm[rows-1][0];
    for(int i=1;i<cols;i++)
    {
        if(mini>costm[rows-1][i])
        {
            mini=costm[rows-1][i];
            vseam[rows-1]=i;
        }
    }
     for(int i=rows-2;i>=0;i--)
        {
            int left=vseam[i+1]-1;
            int mid=vseam[i+1];
            int right=vseam[i+1]+1;
            int ans;
            if(left>=0 && costm[i][left]<costm[i][mid]){
                ans=left;

            }
            else{
                ans=mid;
            }
            if(right<cols && costm[i][ans]>costm[i][right]){
                ans=right;
            }
            vseam[i]=ans;
        }
        for(int i=0;i<rows;i++)
        {
            delete[] costm[i];
        }
        delete[] costm;
        return vseam;
}
int *findhorizantalseam(float **energymatrix,int rows,int cols)
{
    
   int *hseam=new int[cols];
    float **cost;
    cost=new float *[rows];
    for(int i=0;i<rows;i++)
    {
        cost[i]=new float[cols];
    }
    //for first col it is same
    for(int i=0;i<rows;i++)
    {
        cost[i][0]=energymatrix[i][0];
    }
     
    //from second col we need to findout the cost
        
        for(int j=1;j<cols;j++)
        {
            for(int i=0;i<rows;i++)
            {
            if(i==0)
            {
                cost[i][j]=energymatrix[i][j]+min(cost[i][j-1],cost[i+1][j-1]);
            }
            else if(i==rows-1)
            {
                cost[i][j]=energymatrix[i][j]+min(cost[i-1][j-1],cost[i][j-1]);
            }
            else{
                cost[i][j]=energymatrix[i][j]+min(cost[i-1][j-1],min(cost[i][j-1],cost[i+1][j-1]));
            }
        }
    }
     float mini=cost[0][cols-1];
    for(int i=1;i<rows;i++)
    {
        if(mini>cost[i][cols-1])
        {
            mini=cost[i][cols-1];
            hseam[cols-1]=i;
        }
    }
     for(int i=cols-2;i>=0;i--)
        {
            int left=hseam[i+1]-1;
            int mid=hseam[i+1];
            int right=hseam[i+1]+1;
            int ans;
            
            if(left>=0 && cost[left][i]<cost[mid][i]){
                ans=left;
            }
            else{
                ans=mid;
            }
            if(right<rows && cost[ans][i]>cost[right][i]){
                ans=right;
            }
            hseam[i]=ans;
        }
        for(int i=0;i<rows;i++)
        {
            delete[] cost[i];
        }
        delete[] cost;
        return hseam;
}
int main(int argc,char *argv[])
 {
    const char* path = argv[1];
    //Mat is a datatype to store the image matrix 
    Mat img = imread(path,1);
    int new_cols,new_rows;
    new_cols=stoi(argv[2]);
    new_rows=stoi(argv[3]);
    int rows=img.rows,cols=img.cols;
    int ***rgbmat = new int**[rows];
    for(int i =0; i<rows; i++){
    rgbmat[i] = new int*[cols];
    for(int j =0; j<cols; j++){
        rgbmat[i][j] = new int[3];
        for(int k = 0; k<3;k++){
            rgbmat[i][j][k] = 0;
        }
    }
    }
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            for(int k=0;k<3;k++)
            {
                rgbmat[i][j][k]=img.at<Vec3b>(i,j)[k];
            }
        }
    }
    //<....................................Step -1:calculate energy values............................................................> 
    //creating a dynamic array to store the energy values
    float **energyMatrix;
    energyMatrix=new float *[rows];
    for(int i=0;i<rows;i++)
    {
        energyMatrix[i]=new float[cols];
    }
    
        //<.............................................................................................>//
    while(cols!=new_cols)
    {
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {   
            energyMatrix[i][j]=calcEnergypix(rgbmat,i,j,rows,cols); 
        }
    }
    //<...................................find vertical seam........................................>
    int *res=findverticalseam(energyMatrix,rows,cols);
    // for(int i=0;i<rows;i++) cout<<res[i]<<endl;
    // fflush(stdout);
    for(int i=0;i<rows;i++)
    {
        int col=res[i];
        
        for(int j=col;j<cols-1;j++)
        {
            rgbmat[i][j][0]=rgbmat[i][j+1][0];
            rgbmat[i][j][1]=rgbmat[i][j+1][1];
            rgbmat[i][j][2]=rgbmat[i][j+1][2];
        } 
    }
    
    cols=cols-1;
    }
    while(rows!=new_rows)
    {
        for(int i=0;i<rows;i++)
        {
        for(int j=0;j<cols;j++)
        {
            energyMatrix[i][j]=calcEnergypix(rgbmat,i,j,rows,cols);
        }
        }
    //<...................................find horizontal seam........................................>
    int *res=findhorizantalseam(energyMatrix,rows,cols);
    
    for(int j=0;j<cols;j++)
    {
    int row=res[j];
    for(int i=row;i<rows-1;i++)
    { 
            rgbmat[i][j][0]=rgbmat[i+1][j][0];
            rgbmat[i][j][1]=rgbmat[i+1][j][1];
            rgbmat[i][j][2]=rgbmat[i+1][j][2];
        }
    }
    
    rows=rows-1;
    }
     Mat ansimg(rows,cols,CV_8UC3);
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            for(int k=0;k<3;k++)
            {
                ansimg.at<Vec3b>(i,j)[k]=rgbmat[i][j][k];
            }
        }
    }
   
    // to store the image
    imwrite("output.jpeg",ansimg);
    return 0;
}
