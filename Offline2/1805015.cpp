#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;
typedef long long ll;

int value_order_heuristic_type;
int nodenum=0,bt=0,forward_checking=0;
class Cell
{
public:
    int domain;
    int value;

    Cell()
    {

    }

    Cell(int v,int sz)
    {
        value=v;
        if(v==0)
        {
            for(int i=0;i<sz;i++)insert_domain(i+1);
        }
        else
        {
            insert_domain(v);
        }

    }

    void insert_domain(int x)
    {
        domain=domain|(1<<(x-1));
    }

    void remove_domain(int x)
    {
        domain=domain&(~(1<<(x-1)));
    }

    bool isEmpty()
    {
        return domain==0;
    }

    int getSize()
    {
         int x=domain,cnt=0;
         while(x!=0)
         {
             cnt+=(x&1);
             x=x>>1;
         }
         return cnt;
    }

};
class CSP
{
public:
    int sz=0;int n;
    int unassigned;
    vector<vector<Cell> >g;

    CSP(int n,vector<vector<int> >&v)
    {
      sz=n;unassigned=0;
      g=creategrid(v);
    }

    vector<vector<Cell> >creategrid(vector<vector<int> >&v)
    {
        vector<vector<Cell> > grid(sz, vector<Cell>(sz) );
        for(int i=0;i<sz;i++)
        {
            for(int j=0;j<sz;j++)
            {
                Cell c(v[i][j],sz);
                grid[i][j]=c;
            }
        }

        for(int i=0;i<sz;i++)
        {
            for(int j=0;j<sz;j++)
            {
                if(grid[i][j].value==0)unassigned++;
                for(int k=0;k<sz;k++)
                {
                    if(k!=j && grid[i][k].value!=0)grid[i][j].remove_domain(grid[i][k].value);
                    if(k!=i && grid[k][j].value!=0)grid[i][j].remove_domain(grid[k][j].value);
                }

            }
        }
        return grid;
    }

    bool assign(int i,int j,int value)
    {
        bool b=0;
        g[i][j].value=value;unassigned--;
        for(int k=0;k<sz;k++)
        {
            g[i][k].remove_domain(value);
            g[k][j].remove_domain(value);
            if((g[i][k].value==0 && g[i][k].isEmpty()) || (g[k][j].value==0 && g[k][j].isEmpty()) )
            {
                b=1;
            }
        }
        return b;
    }

    void remove(int i,int j,int value)
    {
        g[i][j].value=0;unassigned++;
        for(int k=0;k<sz;k++)
        {

            int b=0;
            for(int k1=0;k1<sz;k1++)
            {
                if(g[i][k1].value==value)b=1;
                if(g[k1][k].value==value)b=1;
            }
            if(b==0)g[i][k].insert_domain(value);

            b=0;
            for(int k1=0;k1<sz;k1++)
            {
                if(g[k][k1].value==value)b=1;
                if(g[k1][j].value==value)b=1;
            }
            if(b==0)g[k][j].insert_domain(value);
        }
    }

    bool is_complete()
    {
        return unassigned==0;
    }

};

class Solution
{
public:

    vector<vector<Cell> >grid;
    int failure;

    Solution(vector<vector<Cell> >&v,int f)
    {
        grid=v;failure=f;
    }

    Solution(int f)
    {
        failure=f;
    }

    void print()
    {
        if(failure==1)
        {
            cout<<"No solution"<<endl;
            return;
        }
        for(int i=0;i<grid.size();i++)
        {
            for(int j=0;j<grid.size();j++)
            {
                cout<<grid[i][j].value<<" ";
            }
            cout<<endl;
        }

    }
};

class CSP_solver
{
public:

   Solution solve(CSP c)
   {
        nodenum++;

        if(c.is_complete())
        {
            Solution s(c.g,0);
            return s;
        }
        pair<int,int>p=variable_order_heuristic(c,value_order_heuristic_type);

        vector<int>values=value_order_heuristic(c,p.first,p.second);
        int temp=1;
        for(int k=0;k<values.size();k++)
        {
             bool b=c.assign(p.first,p.second,values[k]);
             if(forward_checking*b==1)
             {
                 c.remove(p.first,p.second,values[k]);
                 continue;
             }

             temp=0;
             Solution result=solve(c);
             if(result.failure!=1)
             {
                 return result;
             }
             c.remove(p.first,p.second,values[k]);
        }

        if(temp==1)bt++;
        Solution s(1);
        return s;

   }

   pair<int,int> variable_order_heuristic(CSP c,int type)
   {
       if(type==1)
       {
           int mn=1e6;int k1,k2;
           for(int i=0;i<c.sz;i++)
           {
               for(int j=0;j<c.sz;j++)
               {
                   if(c.g[i][j].value==0 && c.g[i][j].getSize()<=mn)
                   {
                       k1=i;k2=j;mn=c.g[i][j].getSize();
                   }
               }
           }
           return {k1,k2};
       }
       else if(type==2)
       {
           int mx=-1e6;int k1,k2;
           for(int i=0;i<c.sz;i++)
           {
               for(int j=0;j<c.sz;j++)
               {
                   if(c.g[i][j].value==0)
                   {
                       int cnt=0;
                       for(int k=0;k<c.sz;k++)
                       {
                           if(c.g[i][k].value==0)cnt++;
                           if(c.g[k][j].value==0)cnt++;
                       }
                       cnt=cnt-2;
                       if(cnt>=mx)
                       {
                           k1=i;k2=j;mx=cnt;
                       }
                   }
               }
           }
           return {k1,k2};
       }
       else if(type==3)
       {
           int mx=-1e6,mn=1e6;int k1,k2;
           for(int i=0;i<c.sz;i++)
           {
               for(int j=0;j<c.sz;j++)
               {
                   if(c.g[i][j].value==0)
                   {
                       int cnt=0;
                       for(int k=0;k<c.sz;k++)
                       {
                           if(c.g[i][k].value==0)cnt++;
                           if(c.g[k][j].value==0)cnt++;
                       }
                       cnt=cnt-2;
                       if(c.g[i][j].getSize()<mn || c.g[i][j].getSize()==mn && cnt>=mx)
                       {
                           k1=i;k2=j;mx=cnt;mn=c.g[i][j].getSize();
                       }
                   }
               }
           }
           return {k1,k2};
       }
       else if(type==4)
       {
           double mn=1e6;int k1,k2;
           for(int i=0;i<c.sz;i++)
           {
               for(int j=0;j<c.sz;j++)
               {
                   if(c.g[i][j].value==0)
                   {
                       int cnt=0;
                       for(int k=0;k<c.sz;k++)
                       {
                           if(c.g[i][k].value==0)cnt++;
                           if(c.g[k][j].value==0)cnt++;
                       }
                       cnt=cnt-1;
                       double d=(double)c.g[i][j].getSize()/cnt;
                       if(d<=mn)
                       {
                           k1=i;k2=j;mn=d;
                       }
                   }
               }
           }
           return {k1,k2};
       }
       else
       {
           int cnt=0;
           for(int i=0;i<c.sz;i++)
           {
               for(int j=0;j<c.sz;j++)
               {
                   if(c.g[i][j].value==0)
                   {
                       cnt++;
                   }
               }
           }
           srand(time(0));
           int ind=(rand()%cnt)+1;
           cnt=0;
           for(int i=0;i<c.sz;i++)
           {
               for(int j=0;j<c.sz;j++)
               {
                   if(c.g[i][j].value==0)
                   {
                       cnt++;
                       if(cnt==ind)return {i,j};
                   }
               }
           }
       }

   }

   vector<int> value_order_heuristic(CSP c,int i ,int j)    /* least constraining value heuristic */
   {
       vector<int>v;
       vector<pair<int,int> >v1;

       for(int k=0;k<c.sz;k++)
       {
           int x=((c.g[i][j].domain)>>k)&1;
           if(x == 0)continue;
           int cnt=0;
           for(int k1=0;k1<c.sz;k1++)
           {
               if(c.g[i][k1].value==0 && (c.g[i][k1].domain>>k)&1 == 1)cnt++;
               if(c.g[k1][j].value==0 && (c.g[k1][j].domain>>k)&1 == 1)cnt++;
           }
           v1.push_back({cnt,k+1});
       }
       sort(v1.begin(),v1.end());
       for(int i=0;i<v1.size();i++)v.push_back(v1[i].second);
       return v;

   }
};

main()
{

    int i,j,n;
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
            }
            else
            {
                grid[i-1][j-1]=stoi(s);
            }
        }
    }
    cout<<endl;

    CSP c(n,grid);

    CSP_solver solver;

    cout<<"Backtracking:"<<endl;
    for(int i=1;i<=5;i++)
    {
        cout<<"VAH"<<i<<":"<<endl<<endl;
        value_order_heuristic_type=i;
        nodenum=bt=0;
        //if(i==2)continue;
        auto start = high_resolution_clock::now();
        Solution s=solver.solve(c);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "duration:" << duration.count() << endl;

        s.print();
        cout<<"nodenum: "<<nodenum<<endl;
        cout<<"bt: "<<bt<<endl;
        cout<<endl<<endl;
    }

    forward_checking=1;
    cout<<"Forward checking:"<<endl;
    for(int i=1;i<=5;i++)
    {
        cout<<"VAH"<<i<<":"<<endl<<endl;
        value_order_heuristic_type=i;
        nodenum=bt=0;
        //if(i==2 || i==5)continue;
        auto start = high_resolution_clock::now();
        Solution s=solver.solve(c);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "duration:" << duration.count() << endl;

        s.print();
        cout<<"nodenum: "<<nodenum<<endl;
        cout<<"bt: "<<bt<<endl;
        cout<<endl<<endl;
    }


}

/*
5
0 2 0 0 0
0 0 3 5 0
0 0 0 0 0
4 0 0 0 0
0 3 0 0 5

3
1 0 3
0 0 1
3 1 0

10
1 0 0 5 0 0 10 6 0 8
0 3 5 0 7 0 0 8 0 0
0 8 2 0 0 3 0 10 0 0
0 2 3 0 10 0 0 0 0 6
3 0 0 9 1 0 0 0 10 5
10 5 8 0 0 0 0 0 3 7
0 0 0 3 8 0 0 5 0 10
0 10 0 2 0 5 1 0 0 0
0 0 0 0 5 10 3 0 6 0
2 0 0 0 0 6 5 0 7 0


*/
