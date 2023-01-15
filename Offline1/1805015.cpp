#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define mod 1000000007
int n;
int blank_row,blank_col,explored=0,expanded=0;
string heuristic;
vector<vector<int> >finalgrid;

int total_move=0;
int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};

struct node
{
    vector<vector<int> >grid;
    node *par;
    int move;
    int priority;

    node(vector<vector<int> >v1,node *n,int m,int p)
    {
        grid=v1;
        par=n;
        move=m;
        priority=p;
    }

};

struct NodeComparer
{
    bool operator()(node* const& node1, node* const& node2)
    {
        return node1->priority > node2->priority;
    }
};

map<vector<vector<int> >,bool>mp;


bool gridchecker(vector<vector<int> >& v)
{
    int cnt=0;
    bool b[n*n+1];
    memset(b,0,sizeof(b));
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(v[i][j]==0)continue;
            b[v[i][j]]=1;
            for(int k=v[i][j]+1;k<=n*n;k++)
            {
                cnt+=b[k];
            }
        }
    }

    if(n%2==1)
    {
        return 1-cnt%2;
    }
    else
    {
        return cnt%2==(n-blank_row-1)%2;
    }
}

int hamming_distance(vector<vector<int> >& v)
{
    int cell_no=1,distance=0;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(v[i][j]!=0 && cell_no!=v[i][j])
            {
                distance++;
            }
            cell_no++;
        }
    }
    return distance;
}

int manhattan_distance(vector<vector<int> >& v)
{
    int distance=0;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(v[i][j]!=0)
            {
                distance+=abs(i-(v[i][j]-1)/n)+abs(j-(v[i][j]-1)%n);
            }
        }
    }
    return distance;
}

int h(vector<vector<int> >& v)
{
    if(heuristic=="hamming")
    {
        return hamming_distance(v);
    }
    else
    {
        return manhattan_distance(v);
    }
}

vector<vector<int> > create_finalgrid()
{
    vector<vector<int> >f(n,vector<int>(n,0));
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            f[i][j]=i*n+j+1;
        }
    }
    f[n-1][n-1]=0;
    return f;
}

void printgrid(vector<vector<int> >& v)
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(v[i][j]==0 )cout<<"* ";
            else cout<<v[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}


void printsteps(node *n,int move_no)
{
    if(n==nullptr)
    {
        return;
    }

    printsteps(n->par,move_no-1);

    if(move_no==0)
    {
        cout<<"Initial Board: "<<endl;
    }
    else
    {
        cout<<"Move "<<move_no<<":"<<endl;
    }
    printgrid(n->grid);
}

void Astar(vector<vector<int> >& v)
{
    priority_queue<node*,vector<node*>,NodeComparer > q;

    node* firstnode=new node(v,nullptr,0,h(v));
    q.push(firstnode);
    expanded++;
    mp[v]=1;

    while(!q.empty())
    {
        node* top=q.top();
        q.pop();
        explored++;
        if(top->grid==finalgrid)
        {
            cout<<"Number of moves: "<<top->move<<endl;

            cout<<"Steps:\n";
            printsteps(top,top->move);

            cout<<"No. of explored nodes: "<<explored<<endl;
            cout<<"No. of expanded nodes: "<<expanded<<endl;
            break;
        }
        vector<vector<int> >grid=top->grid;

        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(grid[i][j]!=0)continue;
                for(int k=0;k<4;k++)
                {
                    int r=i+dx[k];
                    int c=j+dy[k];
                    if(r<0 || r>=n || c<0 || c>=n)continue;

                    vector<vector<int> >newgrid=grid;

                    newgrid[i][j]=grid[r][c];
                    newgrid[r][c]=0;
                    if(mp[newgrid])continue;
                    node* newnode=new node(newgrid,top,top->move+1,h(newgrid)+top->move+1);
                    q.push(newnode);
                    mp[newgrid]=1;
                    expanded++;

                }
            }
        }
    }
}

main()
{
    int i,j;
    cin>>n;

    vector<vector<int> >grid(n,vector<int>(n,0));

    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            string s;
            cin>>s;
            if(s=="*")
            {
                grid[i-1][j-1]=0;
                blank_row=i-1;
            }
            else
            {
                grid[i-1][j-1]=stoi(s);
            }
        }
    }
    cout<<endl;

    if(!gridchecker(grid))
    {
        cout<<"Puzzle not solvable"<<endl<<endl;
        return 0;
    }

    cout<<"Puzzle solvable"<<endl<<endl;

    //cout<<hamming_distance(grid)<<" "<<manhattan_distance(grid)<<endl;
    finalgrid=create_finalgrid();

    cout<<"Heuristic: Hamming distance\n";
    heuristic="hamming";
    Astar(grid);

    total_move=explored=expanded=0;
    mp.clear();

    cout<<"Heuristic: Manhattan distance\n";
    heuristic="manhattan";
    Astar(grid);



}
/*
3
7 8 *
1 2 3
4 5 6

3
* 1 3
4 2 5
7 8 6

*/
