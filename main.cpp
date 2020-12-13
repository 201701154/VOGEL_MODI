/*
    NAME : KARAN JHAVERI
    ID : 201701154
    COURSE : OPTIMIZATION
    LAB ASSIGNMENT 3
*/
#include<bits/stdc++.h>
using namespace std;
#define int long long

void cal_row_penalty(vector<vector<int>> &c,vector<bool> &fr,vector<bool> &fc,int factories,vector<int> &rp)
{
    int n=factories,m=fc.size();
    
    /* CALCULATING ROW PENALTY FOR ALL REMAINING ROWS */
    for(int i=0;i<n;i++)
    {
        if(fr[i]==true) 
        {
            rp[i]=-1;
            continue;
        }
        vector<int> temp;
        for(int j=0;j<m;j++)
        {
            if(fc[j]==false)
            {
                temp.push_back(c[i][j]);
            }
        }
        sort(temp.begin(),temp.end());
        if(temp.size() >= 2)
        {
            rp[i]=temp[1]-temp[0];
        }
        else if(temp.size() == 1)
        {
            rp[i]=temp[0];
        }
        else rp[i]=-1;
    }
}

void cal_col_penalty(vector<vector<int>> &c,vector<bool> &fr,vector<bool> &fc,int warehouses,vector<int> &cp)
{
    int n=warehouses,m=fr.size();
    
    /* CALCULATING COLUMN PENALTY FOR ALL REMAINING COLUMNS */
    for(int i=0;i<n;i++)
    {
        if(fc[i]==true) 
        {
            cp[i]=-1;
            continue;
        }
        vector<int> temp;
        for(int j=0;j<m;j++)
        {
            if(fr[j]==false)
            {
                temp.push_back(c[j][i]);
            }
        }
        sort(temp.begin(),temp.end());
        if(temp.size() >= 2)
        {
            cp[i]=temp[1]-temp[0];
        }
        else if(temp.size() == 1)
        {
            cp[i]=temp[0];
        }
        else cp[i]=-1;
    }
}

int cal_max(vector<int> v, int n)
{
    int maxi=-1;
    for(int i=0;i<n;i++)
    {
        maxi=max(maxi,v[i]);
    }
    return maxi;
}

pair<int,int> cal_allocation(vector<vector<int>> &c, vector<int> &s,vector<int> &d,vector<bool> &fr,vector<bool> &fc,int p,int q)
{
    int n=s.size(),m=d.size(),mini=INT_MAX,count=0,maxi=0,ind=-1;
    
    /* FOR EACH ROW AND COLUMN WHERE row_penalty[i] = maximum OR column_penalty[i] = maximum, FINDING 
        ROW OR COLUMN WHERE MAXIMUM ALLOCATION IS POSSIBLE*/
    if(q==-1)
    {
        for(int i=0;i<m;i++)
        {
            if(fc[i]==false)
                mini=min(mini,c[p][i]);
        }
        for(int i=0;i<m;i++)
        {
            if(mini==c[p][i])
            {
                int temp=min(s[p],d[i]);
                if(maxi<temp)
                {
                    maxi=temp;
                    ind=i;
                }
            }
        }
    }
    else
    {
        for(int i=0;i<n;i++)
        {
            if(fr[i]==false)
                mini=min(mini,c[i][q]);
        }
        for(int i=0;i<n;i++)
        {
            if(mini==c[i][q])
            {
                int temp=min(s[i],d[q]);
                if(maxi<temp)
                {
                    maxi=temp;
                    ind=i;
                }
            }
        }
    }
    return {maxi,ind};
}

void cal_location(vector<vector<int>> &cost, vector<int> &supply, vector<int>&demand, vector<int> &row_penalty,vector<int> &column_penalty,
                        int factories,int warehouses,int &maximum,int &i_iter,int &j_iter,int maxi,vector<bool> &fr, vector<bool> &fc)
{
    /*FINDING LOCATION OF POINT ALONG WITH HOW MUCH TO ALLOCATE AT THAT POINT */
    for(int i=0;i<factories;i++)
    {
        if(row_penalty[i]==maxi)
        {
            pair<int,int> p = cal_allocation(cost,supply,demand,fr,fc,i,-1);
            int allo = p.first,index=p.second;
            if(allo>maximum)
            {
                maximum=allo;
                i_iter=i;
                j_iter=index;
            }
            
        }
    }
    
    for(int i=0;i<warehouses;i++)
    {
        if(column_penalty[i]==maxi)
        {
            pair<int,int> p = cal_allocation(cost,supply,demand,fr,fc,-1,i);
            int allo = p.first,index=p.second;
            if(allo>maximum)
            {
                maximum=allo;
                i_iter=index;
                j_iter=i;
            }
        }
    }
}

pair<int,char> cal_max_allo(vector<int> &u,vector<int> &v,vector<vector<int>> &output)
{
    int n=u.size(),m=v.size(),maxi1=0,ind1=-1,maxi2=0,ind2=-1;
    
    /* FINDING ROW OR COLUMN WHERE MAXIMUM ALLOCATIONS ARE MADE */
    for(int i=0;i<n;i++)
    {
        int count=0;
        for(int j=0;j<m;j++)
        {
            if(output[i][j]>=0) count++;
        }
        if(maxi1<count)
        {
            maxi1 = count;
            ind1 = i;
        }
    }
    for(int j=0;j<m;j++)
    {
        int count=0;
        for(int i=0;i<n;i++)
        {
            if(output[i][j]>=0) count++;
        }
        if(maxi2<count)
        {
            maxi2 = count;
            ind2 = j;
        }
    }
    if(maxi1>maxi2) return {ind1,'r'};
    return {ind2,'c'};
}

void cal_u_v(vector<int> &u,vector<int> &v,vector<vector<int>> &output,vector<vector<int>> &cost)
{
    pair<int,char> p = cal_max_allo(u,v,output);
    if(p.second=='r') u[p.first] = 0;
    else v[p.first] = 0;
    
    /* CALCULATING u-v VECTORS */
    int rem = true,n=u.size(),m=v.size();
    while(rem)
    {
        int count = 0;
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<m;j++)
            {
                if(output[i][j]>=0)
                {
                    if(u[i]!=INT_MAX && v[j]==INT_MAX)
                    {
                        v[j] = cost[i][j] - u[i];
                    }
                    else if(u[i]==INT_MAX && v[j]!=INT_MAX)
                    {
                        u[i] = cost[i][j] - v[j];
                    }
                }
            }
        }
        
        for(int i=0;i<n;i++)
        {
            if(u[i] != INT_MAX) count++;
        }
        for(int j=0;j<m;j++)
        {
            if(v[j] != INT_MAX) count++;
        }
        
        if(count == n+m) rem = false;
    }
}

bool cal_opportunity_cost(vector<vector<int>> &delta,vector<vector<int>> &output,vector<vector<int>> &cost,vector<int> &u,vector<int> &v)
{
    int n=u.size(),m=v.size();
    bool flag = false;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(output[i][j] < 0)
            {
                delta[i][j] = cost[i][j] - (u[i] + v[j]); //CALCULATING OPPORTUNITY COST 
                if(delta[i][j] < 0) flag = true;
            }
        }
    }
    return flag;
}

pair<int,int> find_location(vector<vector<int>> &delta,int n,int m)
{
    int mini=INT_MAX,p=0,q=0;
    
    /* FINDING POINT WHERE delta(i,j) IS MOST NEGATIVE */
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(delta[i][j] < 0)
            {
                if(mini > delta[i][j])
                {
                    mini = delta[i][j];
                    p=i;
                    q=j;
                }
            }
        }
    }
    return {p,q};
}


void find_loop(pair<int,int> loc, vector<vector<int>> &output)
{
    int n = output.size(),m = output[0].size(),lc=0,mi=loc.first,mj=loc.second,i,j,k;
    vector<vector<int>> loop(n,vector<int>(m,0));
    vector<vector<char>> sign(n,vector<char>(m,'/'));
    
    /* FINDING THE LOOP */
    for(i=0;i<n;i++)
    {
    	 for(j=0;j<m;j++)
    	 {
              if(output[i][j]>0)
        	        loop[i][j]=1;
              else
        	        loop[i][j]=0;
              
              sign[i][j]='a';
         }
    }
    
    for(k=0;k<n;k++)
    {
        for(i=0;i<n;i++)
        {
            for(j=0;j<m;j++)
            {
                if(loop[i][j]==1)
                    lc++;
            }       
            if(lc==1 && i!=mi)
            {
                for(j=0;j<m;j++)
    	            loop[i][j]=0;
            }
          lc=0;
        }

        lc=0;
        for(j=0;j<m;j++)
        {
            for(i=0;i<n;i++)
            {
                if(loop[i][j]==1)
	                lc++;
            }
            if(lc==1 && j!=mj)
            {
                for(i=0;i<n;i++)
	                loop[i][j]=0;
            }
            lc=0;
        }
    }
    
    loop[mi][mj]=1;
    sign[mi][mj]='+';
    i=mi,j=0;
    
    /* CALCULATING SIGNS */
	for(k=1;k<=n-1;k++)
	{
         for(j=0;j<m;j++)
         {
        	  if(loop[i][j]==1 && sign[i][j]=='a')
        	  {
        	    sign[i][j]='-';
        	    break;
              }
         }
         for(i=0;i<n;i++)
         {
        	  if(loop[i][j]==1 && sign[i][j]=='a')
        	  {
            	sign[i][j]='+';
            	break;
        	  }
         }
	}
	
	/* FINDING MINIMUM ALLOCTED VALUE AMONG ALL '-' SIGNS */ 
	int mini = INT_MAX;
	for(int i=0;i<n;i++)
	{
	     for(int j=0;j<m;j++)
    	 {
            if(sign[i][j] == '-')
                mini=min(mini,output[i][j]);
         }
    }
    
    /* UPDATING THE OUTPUT MATRIX */
    output[mi][mj] = 0;
	for(int i=0;i<n;i++)
	{
	     for(int j=0;j<m;j++)
    	 {
            if(sign[i][j] == '+')
            {
                output[i][j] += mini;
            }
            else if(sign[i][j] == '-')
            {
                output[i][j] -= mini;
                if(output[i][j]==0) output[i][j] = -1;
            }
         }
    }
	
}

void solve()
{
    /*INPUT MATRIX INDICATING THE COST */
    vector<vector<int>> cost={{19,30,50,10},{70,30,40,60},{40,8,70,20}};
    
    /*INPUT VECTOR A INDICATING THE SUPPLY CAPACITY OF EACH FACTORY */
    vector<int> supply={7,9,18};
    
    /*INPUT VECTOR B INDICATING THE DEMAND CAPACITY OF EACH WAREHOUSE */
    vector<int> demand={5,8,7,14};
    
    
    /*                                     VOGEL'S APPROXIMATION METHOD (VAM) METHOD                   */
    
    int factories = cost.size(); // TOTAL FACTORIES I.E. NUMBER OF ROWS
    int warehouses = cost[0].size(); // TOTAL WAREHOUSES I.E. NUMBER OF COLUMNS

    /*CALCULATING TOTAL SUPPLY AND TOTAL DEMAND*/
    int sum_supply = 0, sum_demand = 0;
    for(int i=0;i<factories;i++) sum_supply += supply[i];
    for(int i=0;i<warehouses;i++) sum_demand += demand[i];
    
    if(sum_demand > sum_supply) // TOTAL DEMAND MORE THAN TOTAL SUPPLY
    {
        cout<<"DEMAND IS MORE THAN SUPPLY.\nDUMMY ROW IS ADDED\n";
        int diff = sum_demand - sum_supply;
        supply.push_back(diff);
        factories++;
        vector<int> temp;
        for(int i=0;i<warehouses;i++) temp.push_back(0);
        cost.push_back(temp);
        
    }
    else if(sum_demand < sum_supply) // TOTAL SUPPLY MORE THAN TOTAL DEMAND
    {
        cout<<"SUPPLY IS MORE THAN DEMAND.\nDUMMY COLUMN IS ADDED\n";
        int diff = sum_supply - sum_demand;
        demand.push_back(diff);
        warehouses++;
        for(int i=0;i<factories;i++) cost[i].push_back(0);
    }
    
    cout<<"INPUT: \nCOST MATRIX: \n";
    for(int i=0;i<factories;i++)
    {
        for(int j=0;j<warehouses;j++)
            cout<<cost[i][j]<<" ";
        cout<<"\n";    
    }
    cout<<"SUPPLY CAPACITY FOR EACH FACTORY: \n";
    for(int i=0;i<factories;i++)
        cout<<supply[i]<<" ";
    cout<<"\nDEMAND CAPACITY FOR EACH WAREHOUSE: \n";    
    for(int i=0;i<warehouses;i++)
        cout<<demand[i]<<" ";
    cout<<"\n\n";    
        
    vector<vector<int> > output(factories,vector<int>(warehouses, -1)); // OUTPUT MATRIX INDICATING QUANTITIES ASSOCIATED WITH WAREHOUSE
    vector<int> row_penalty(factories,-1); // VECTOR FOR CALCULATING ROW PENALTY
    vector<int> column_penalty(warehouses,-1); // VECTOR FOR CALCULATING COLUMN PENALTY
    
    vector<bool> flag_row(factories,false); // KEEPING TRACK OF WHICH ROW IS LEFT TO CONSIDER
    vector<bool> flag_col(warehouses,false); // KEEPING TRACK OF WHICH COLUMN IS LEFT TO CONSIDER
    int finish = 1,till=factories+warehouses-1;
    if((sum_demand > sum_supply) || (sum_demand < sum_supply)) till--;

    while(finish++ <= till)
    {
        
        cal_row_penalty(cost,flag_row,flag_col,factories,row_penalty); // FUNCTION FOR CALCULATING ROW PENALTY
        cal_col_penalty(cost,flag_row,flag_col,warehouses,column_penalty); // FUNCTION FOR CALCULATING COLUMN PENALTY
        
        int max_in_row = cal_max(row_penalty,factories);
        int max_in_col = cal_max(column_penalty,warehouses);
        int maxi=max(max_in_row,max_in_col); // FINDING THE MAXIMUM AMONG ALL OBTAINED PENALTIES
        
        int i_iter=-1,j_iter=-1,maximum=0;
        
        /* FINDING THE CELL WHERE MAXIMUM ALLOCATION IS POSSIBLE */
        cal_location(cost,supply,demand,row_penalty,column_penalty,factories,warehouses,maximum,i_iter,j_iter,maxi,flag_row,flag_col);
    
        /* MODIFYING CERTAIN QUANTITIES FOR FURTHER EVALUATION */ 
        if(maximum == demand[j_iter])
        {
            int temp = supply[i_iter];
            supply[i_iter] -= maximum;
            demand[j_iter] = 0;
            flag_col[j_iter] = true;
            column_penalty[j_iter] = -1;
            if(maximum == temp)
            {
                supply[i_iter] = 0;
                demand[j_iter] -= maximum;
                flag_row[i_iter] = true;
                row_penalty[i_iter] = -1;
            }
        }
        else
        {
            supply[i_iter] = 0;
            demand[j_iter] -= maximum;
            flag_row[i_iter] = true;
            row_penalty[i_iter] = -1;
        }
        
        output[i_iter][j_iter] = maximum;
        
        bool f=false;  // CHECKING WHETHER ALL ROWS AND ALL COLUMNS ARE VISITED OR NOT
        for(int i=0;i<factories;i++)
            if(flag_row[i]==false) f=true;
        for(int i=0;i<warehouses;i++)
            if(flag_col[i]==false) f=true;
        
        if(f==false) break;
        
    }
    
    cout<<"OUTPUT: \n";
    cout<<"INITIAL BASIC FEASIBLE SOLUTION: \n";
    int total_cost = 0,count=0,mini=INT_MAX,p=0,q=0;
    for(int i=0;i<factories;i++)
    {
        for(int j=0;j<warehouses;j++)
        {
            if(output[i][j]>=0)
                cout<<"X"<<i<<j<<"= "<<output[i][j]<<"\n";
            if(output[i][j]>=0)
            {
                count++;
                total_cost += output[i][j]*cost[i][j]; // CALCUTING TOTAL COST
            }
            else
            {
                if(mini>cost[i][j] && cost[i][j] > 0) // CALCULATING MINIMUM AMONG ALL UNOCCUPIED CELLS FOR MODI METHOD
                {
                    mini = cost[i][j];
                    p=i;
                    q=j;
                }
            }
        }
    }
    
    cout<<"BY VAM METHOD OPTIMUM COST  = "<<total_cost<<endl;
    //cout<<mini<<" ";
    
    if(count == factories + warehouses -1)
        cout<<"SOLUTION IS NON DEGENERATE\n";
    else
    {
        cout<<"SOLUTION IS DEGENERATE AND DUMMY VALUE IS ASSIGNED TO MINIMUM OF UNOCCUPIED CELLS.\n";
        //cout<<p<<" "<<q<<"\n";
        output[p][q] = 0;
    }
    
    /*                                                 MODI METHOD - UV METHOD                                   */
    
    vector<int> u(factories,INT_MAX); // u VECTOR
    vector<int> v(warehouses,INT_MAX); // v VECTOR
    
    /* MATRIX FOR CALCULATING delta(i,j) = cost(i,j) - (u(i) + v(j)) */
    vector<vector<int> > delta(factories,vector<int>(warehouses, INT_MAX));
    
    cal_u_v(u,v,output,cost); // FUNCTION FOR CALCULATING u AND v VECTOR
    
    /* FUNCTION FOR CALCULATING delta(i,j) AND CHECKING WHETHER ALL ENTRIES ARE POSITIVE OR NEGATIVE */
    bool status = cal_opportunity_cost(delta,output,cost,u,v);  
    
    if(status == false)
    {
        cout<<"NO NEED OF OPTIMIZATION,\nOBTAINED INITIAL BASIC FEASIBLE SOLUTION IS OPTIMUM.";
        return;
    }
    
    while(status)
    {
        pair<int,int> loc = find_location(delta,factories,warehouses); // FINDING LOCATION OF POINT WHERE LOOP STARTS
        find_loop(loc,output); // FINDING THE LOOP, SIGN ASSOCIATED WITH CORNER POINTS AND NEW SOLUTION
        cal_u_v(u,v,output,cost);// FUNCTION FOR CALCULATING u AND v VECTOR
        
        /* FUNCTION FOR CALCULATING delta(i,j) AND CHECKING WHETHER ALL ENTRIES ARE POSITIVE OR NEGATIVE */
        status = cal_opportunity_cost(delta,output,cost,u,v); 
        
        fill(u.begin(),u.end(),INT_MAX);
        fill(v.begin(),v.end(),INT_MAX);
    }
    
    cout<<"OPTIMUM SOLUTION USING MODI METHOD: \n";
    total_cost = 0;
    for(int i=0;i<factories;i++)
    {
        for(int j=0;j<warehouses;j++)
        {
            if(output[i][j]>=0)
                cout<<"X"<<i<<j<<"= "<<output[i][j]<<"\n";
            if(output[i][j]<0) output[i][j] = 0;
                total_cost += output[i][j]*cost[i][j];
        }
    }
    cout<<"BY MODI METHOD OPTIMUM COST = "<<total_cost<<endl;
    
}
int32_t main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    solve();
}
