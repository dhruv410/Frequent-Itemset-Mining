#include<bits/stdc++.h>
using namespace std;
#define ff first
#define ss second
#define pb push_back
#define mp make_pair
#define all(a) a.begin(),a.end()
typedef long long ll;
typedef pair <ll, ll> pll;

struct vector_length_compare
{
	bool operator() (vector<ll> lhs, vector<ll> rhs)
	{
		ll lhs_size = lhs.size();
		ll rhs_size = rhs.size();
		if(lhs_size==rhs_size)
			return (lhs<rhs);
		return (lhs_size < rhs_size);
	}
};

bool all_frequent_subsets(vector<ll> candidate, set<vector<ll>> to_check)
{
	int n = candidate.size();
	for(int i=0;i<n;i++)
	{
		vector <ll> temp;
		for(int j=0;j<n;j++)
		{
			if(j==i)
				continue;
			temp.pb(candidate[j]);
		}
		if(to_check.find(temp)==to_check.end())
			return false;
	}
	return true;
}

vector <vector<ll>> apriori_gen(vector<vector<ll> > frequent)
{
	vector<vector<ll> > candidates;
	set<vector<ll>> to_prune;
	for(auto j: frequent)
		to_prune.insert(j);
	int l = frequent[0].size();
	int n = frequent.size();
	for(int i=0;i<n;i++)
	{
		// cout<<i<<endl;
		for(int j=i+1;j<n;j++)
		{
			int k=0;
			for(;k<l;k++)
			{
				if(frequent[i][k]!=frequent[j][k])
				{
					break;
				}
			}
			if(k==l-1)
			{
				vector<ll> temp(all(frequent[i]));
				temp.pb(frequent[j][l-1]);
				if(all_frequent_subsets(temp, to_prune))
				{
					candidates.pb(temp);
				}
			}
		}
	}
	return candidates;
}

vector < vector <ll> > apriori_simple(vector<set<ll> > v, ll min_sup)
{
	auto start1 = std::chrono::system_clock::now();
	cout<<"Apriori-Simple function is called"<<endl;
	vector <vector<ll>> frequent;
	vector <vector<ll>> candidates, candidates2;
	map<ll, ll> itemset_1;
	for(auto j: v)
	{
		for(auto j1 = j.begin();j1!=j.end();j1++)
		{
			itemset_1[*j1]++;
		}
	}
	for(auto j: itemset_1)
	{
		if(j.ss>=min_sup)
		{
			vector<ll> temp(1, j.ff);
			frequent.pb(temp);
			candidates2.pb(temp);
		}
	}
	while(!candidates2.empty())
	{
		candidates = apriori_gen(candidates2);
		candidates2.clear();
		for(auto j: candidates)
		{
			int cnt = 0;
			for(auto j1: v)
			{
				int fl = 1;
				for(auto j2: j)
				{
					if(j1.find(j2)==j1.end())
					{
						fl = 0;
						break;
					}
				}
				if(fl)
					cnt++;
			}
			if(cnt>=min_sup)
			{
				candidates2.pb(j);
				frequent.pb(j);
			}
		}
	}
	auto end1 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end1-start1;
	cout<<"Runtime of the Apriori-Simple function is: ";	
	cout << fixed << setprecision(4) << elapsed.count()<<endl;
	return frequent;
}

vector < vector <ll> > apriori_hash(vector<set<ll> > v, ll min_sup)
{
	auto start1 = std::chrono::system_clock::now();
	cout<<"Hash-Tree function is called"<<endl;
	vector <vector<ll>> frequent;
	vector <vector<ll>> candidates, candidates2;
	map<ll, ll> itemset_1;
	map<pll, ll> itemset_2;
	for(auto j: v)
	{
		for(auto j1 = j.begin();j1!=j.end();j1++)
		{
			itemset_1[*j1]++;
		}
	}
	for(auto j: itemset_1)
	{
		if(j.ss>=min_sup)
		{
			vector<ll> temp(1, j.ff);
			frequent.pb(temp);
		}
	}
	if(frequent.empty())
	{
		auto end1 = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = end1-start1;
		cout<<"Runtime of the hash-tree function is: ";	
		cout << fixed << setprecision(4) << elapsed.count()<<endl;
		return frequent;
	}
	for(auto j: v)
	{
		for(auto j1 = j.begin();j1!=j.end();j1++)
		{
			auto j2 = j1;
			j2++;
			for(;j2!=j.end();j2++)
			{
				itemset_2[mp(*j1, *j2)]++;
			}
		}
	}
	for(auto j: itemset_2)
	{
		if(j.ss>=min_sup)
		{
			vector <ll> temp;
			temp.pb(j.ff.ff);
			temp.pb(j.ff.ss);
			frequent.pb(temp);
			candidates2.pb(temp);
		}
	}
	while(!candidates2.empty())
	{
		candidates = apriori_gen(candidates2);
		candidates2.clear();
		for(auto j: candidates)
		{
			int cnt = 0;
			for(auto j1: v)
			{
				int fl = 1;
				for(auto j2: j)
				{
					if(j1.find(j2)==j1.end())
					{
						fl = 0;
						break;
					}
				}
				if(fl)
					cnt++;
			}
			if(cnt>=min_sup)
			{
				candidates2.pb(j);
				frequent.pb(j);
				// for(auto j1: j)
				// 	cout<<j1<<" ";
				// cout<<"Frequency: "<<cnt<<endl;
			}
		}
	}
	auto end1 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end1-start1;
	cout<<"Runtime of the hash-tree function is: ";	
	cout << fixed << setprecision(4) << elapsed.count()<<endl;
	return frequent;
}

vector<vector<ll> > apriori_partition(vector<set<ll> > v, ll psize, ll min_sup)
{
	auto start1 = std::chrono::system_clock::now();
	cout<<"Partition Apriori function is called"<<endl;
	vector<vector<ll> > ans;
	set<vector<ll>, vector_length_compare > merged;
	ll n = v.size();
	ll ptr = 0, offset = 0;
	while(ptr*psize<n)
	{
		vector<set<ll>> temp;
		ll sz1 = 0;
		for(offset = ptr*psize; offset<min(n, (ptr+1)*psize);offset++)
		{
			temp.pb(v[offset]);
			sz1++;
		}
		ll min_sup1 = (sz1*min_sup)/n; 
		vector<vector<ll>>temp2 = apriori_simple(temp, min_sup1);
		for(auto j: temp2)
		{
			merged.insert(j);
		}
		ptr++;
	}
	for(auto candidate: merged)
	{
		int cnt = 0;
		for(auto record: v)
		{
			int fl = 1;
			for(auto entry: candidate)
			{
				if(record.find(entry)==record.end())
				{
					fl = 0;
					break;
				}
			}
			if(fl)
			{
				cnt++;
			}
		}
		if(cnt>=min_sup)
		{
			ans.pb(candidate);
		}
	}
	auto end1 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end1-start1;
	cout<<"Runtime of the Partition Apriori function is: ";	
	cout << fixed << setprecision(4) << elapsed.count()<<endl;
	return ans;
}

vector<vector<ll> > find_closed(vector<set<ll> > v, vector<vector<ll>> frequent, ll min_sup)
{
	auto start1 = std::chrono::system_clock::now();
	cout<<"Find closed frequent itemset function is called"<<endl;
	vector <vector<ll> > ans;
	map<vector<ll> , ll> m;
	map<vector<ll>, bool> check;
	set<vector<ll>, vector_length_compare > merged;
	for(auto j: frequent)
	{
		check[j] = true;
		for(auto record: v)
		{
			int fl = 1;
			for(auto entry: j)
			{
				if(record.find(entry)==record.end())
				{
					fl = 0; 
					break;
				}
			}
			if(fl)
				m[j]++;
		}
	}
	for(auto j: frequent)
	{
		int l = j.size();
		for(int i=0;i<l;i++)
		{
			vector<ll> temp;
			for(int j1=0;j1<l;j1++)
			{
				if(j1!=i)
					temp.pb(j[j1]);
			}
			if(m.find(temp)!=m.end())
			{
				// cout<<m[j]<<" "<<m[temp]<<endl;
				if(m[temp]==m[j])
				{
					check[temp] = false;
				}
			}
		}
	}	
	for(auto j: check)
	{
		if(j.ss==true)
			merged.insert(j.ff);
	}
	for(auto j: merged)
		ans.pb(j);
	auto end1 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end1-start1;
	cout<<"Runtime of the Find closed frequent itemset function is: ";	
	cout << fixed << setprecision(4) << elapsed.count()<<endl;
	return ans;
}

void display(vector<vector<ll> > ans)
{
	int prev = 0;
	int cnt = 0;
    for(auto j: ans)
    {
    	int tmp  = j.size();
    	if(tmp!=prev)
    	{
    		prev = tmp;
    		if(prev!=1)
    		{
	    		cout<<"]"<<endl;
	    		cout<<"Total count is: "<<cnt<<endl;
    		}
    		cnt = 0;
    		cout<<"[";
    	}
    	cout<<"[";
    	for(int i=0;i<tmp;i++)
    	{
    		if(i!=tmp-1)
    			cout<<j[i]<<", ";
    		else
    			cout<<j[i]<<"]"<<", ";
    	}
    	cnt++;
    }
    cout<<"]"<<endl;
    cout<<"Total count is: "<<cnt<<endl;
}

int main()
{
   	string file_path,line;
	cout << "Input The Path To The File -> ";
	cin >> file_path;
	ifstream fin(file_path, ios::in);
    vector<set<ll>> v;
    int input_size = 0;
    int cnt1 = 0, cnt2 = 0;
    while(getline(fin, line))
    {
        stringstream s(line);
        string word;
        set<ll> t;
        while(getline(s,word,' '))
        {
            int temp = stoi(word);
            if(temp > 0)
            {
                t.insert(temp);
            }
        }
        v.push_back(t);
        input_size++;
    }
    float min_sup_perc;
    ll min_sup;
    cout<<"Enter the minimum support count in percent"<<endl;
    cin>>min_sup_perc;
    min_sup = (min_sup_perc*input_size)/100;
    vector <vector<ll> > ans;

    ans = apriori_simple(v, min_sup);
    cout<<"Frequent itemsets are: "<<endl;
    display(ans);

    ans = apriori_hash(v, min_sup);
    cout<<"Frequent itemsets are: "<<endl;
    display(ans);

    ll psize;
 	cout<<"Enter the partition size: "<<endl;
 	cin>>psize;
 	ans = apriori_partition(v, psize, min_sup);
 	cout<<"Frequent itemsets are: "<<endl;
 	display(ans);


 	ans = find_closed(v, ans, min_sup);
 	cout<<"Closed frequent itemsets are: "<<endl;
 	display(ans);
    return 0;
}