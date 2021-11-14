#include<bits/stdc++.h>
using namespace std;
#define ff first
#define ss second
#define pb push_back
#define mp make_pair
#define all(a) a.begin(),a.end()
typedef long long ll;
typedef pair <ll, ll> pll;

struct node
{
	struct node *parent;
	int freq;
	int val;
	map<ll, node*> child;
};

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

vector<vector<ll> > generate_power_set(set<ll> v1)
{

	vector<ll> v;
	vector<vector<ll> > ans;
	int l = v.size();
	int l1 = pow(2, l);
	for(int i=1;i<=l1;i++)
	{
		vector <ll> temp;
		for(int k=0;k<l;k++)
		{
			if((i&(1<<k))>0)
			{
				temp.pb(v[k]);
			}
		}
		ans.pb(temp);
	}
	return ans;
}
ll lastt = 0;
vector<vector<ll> > fp_grow(vector<set<ll> > v, ll min_sup, set<ll> s)
{

	vector<vector<ll>> ans;
	if(v.size()==0)
	{
		return ans;
	}
	set<vector<ll>, vector_length_compare > merge;
	vector<pll> header;
	map<ll, ll> m;
	map<ll, vector<struct node*> > m1;
	set<ll> left;
	for(auto j: v)
	{
		for(auto j1: j)
		{
			if(s.find(j1)==s.end())
				m[j1]++;
		}
	}
	for(auto j: m)
	{
		if(j.ss>=min_sup)
		{
			header.pb(mp(j.ss, j.ff));
		}
		else
			left.insert(j.ff);
	}
	//checking for base condition
	int fl, cnt = 0;
	for(auto j1: v)
	{
		fl = 1;
		for(auto j: m)
		{
			if(j1.find(j.ff)==j1.end())
			{
				fl = 0;
				break;
			}
		}
		if(!fl)
			cnt++;
	}
	if(fl==1||v.size()==1)
	{
		// base condition reached
		//generate all possible combinations
		// cout<<fl<<endl;
		return generate_power_set(v[cnt]);
	}
	sort(all(header));
	// reverse(all(header));
	// for(auto j: header)
	// 	cout<<j.ss<<" "<<j.ff<<endl;

	struct node *root = new node();
    root->val = -1;
    root->freq = 0;
    root->parent = NULL;
	// cout<<root->val<<endl;
	//constructing the fp-tree
	for(auto j: v)
	{
		vector <pll> seq;
		// struct node *cur = (struct node*)malloc(sizeof(struct node));
		struct node* cur = root;
		// cout<<cur->val<<endl;
		for(auto j1: j)
		{
			ll fr = m[j1]; 
			// auto j2 = lower_bound(all(header), mp(fr, j1));
			// if(j1==(*j2).ss)
			// {
			// 	cout<<"Yes"<<endl;
			// 	auto idx = j2 - header.begin();
			// 	seq.pb(mp(idx, j1));
			// }
			for(int idx = 0;idx<header.size();idx++)
			{
				if(header[idx].ff==fr && header[idx].ss==j1)
					seq.pb(mp(idx, j1));		
			}
		}
		sort(all(seq));
		for(auto j1: seq)
		{
			if((cur->child).find(j1.ss)==(cur->child).end())
			{
				struct node *temp = new node();
				temp->val = j1.ss;
				temp->freq = 1;
				cur->child[j1.ss] = temp;
				temp->parent = cur;
				if(m1.find(j1.ss)==m1.end())
				{
					vector<struct node*> temp2;
					temp2.pb(temp);
					m1[j1.ss] = temp2;
				}
				else
				{
					m1[j1.ss].pb(temp);
				}
			}
			else
			{	
				cur->child[j1.ss]->freq++;
			}
			cur = cur->child[j1.ss];
		}
		// cout<<endl;
	}
	int l1 = header.size();
	for(int i=0;i<l1;i++)
	{
		ll last_element = header[i].ss;
		// lastt = last_element;
		s.insert(last_element);
		vector<set<ll> > v_temp;
		// inserting all the prefixes of last_element in fp-tree in v_temp;

		for(auto j: m1[last_element])
		{
			auto temp = j->parent;
			set<ll> s1;
			while(temp->val!=-1)
			{
				s1.insert(temp->val);
				temp = temp->parent;
			}
			for(int j1=0;j1<j->freq;j1++)
				v_temp.pb(s1);
		}
		vector<vector<ll>> nans = fp_grow(v_temp, min_sup, s);
		for(auto j: nans)
		{
			j.pb(last_element);
			set<ll> f_temp;
			for(auto j1: j)
				f_temp.insert(j1);
			j.clear();
			for(auto j1: f_temp)
				j.pb(j1);
			merge.insert(j);
		}
		vector<ll> tp;
		tp.push_back(last_element);
		merge.insert(tp);
		s.erase(last_element);	
	}
	for(auto j: merge)
		ans.pb(j);
	return ans;
}

vector<vector<ll> > fp_grow_merge(vector<set<ll> > v, ll min_sup, set<ll> s)
{

	vector<vector<ll>> ans;
	if(v.size()==0)
	{
		return ans;
	}
	set<vector<ll>, vector_length_compare > merge;
	vector<pll> header;
	map<ll, ll> m;
	map<ll, vector<struct node*> > m1;
	set<ll> left;
	for(auto j: v)
	{
		for(auto j1: j)
		{
			if(s.find(j1)==s.end())
				m[j1]++;
		}
	}
	for(auto j: m)
	{
		if(j.ss>=min_sup)
		{
			header.pb(mp(j.ss, j.ff));
		}
		else
			left.insert(j.ff);
	}
	//checking for base condition
	int fl, cnt = 0;
	for(auto j1: v)
	{
		fl = 1;
		for(auto j: m)
		{
			if(j1.find(j.ff)==j1.end())
			{
				fl = 0;
				break;
			}
		}
		if(!fl)
			cnt++;
	}
	if(fl==1||v.size()==1)
	{
		// base condition reached
		//generate all possible combinations
		// cout<<fl<<endl;
		return generate_power_set(v[cnt]);
	}
	sort(all(header));
	// reverse(all(header));
	// for(auto j: header)
	// 	cout<<j.ss<<" "<<j.ff<<endl;

	struct node *root = new node();
    root->val = -1;
    root->freq = 0;
    root->parent = NULL;
	// cout<<root->val<<endl;
	//constructing the fp-tree
	for(auto j: v)
	{
		vector <pll> seq;
		// struct node *cur = (struct node*)malloc(sizeof(struct node));
		struct node* cur = root;
		// cout<<cur->val<<endl;
		for(auto j1: j)
		{
			ll fr = m[j1]; 
			// auto j2 = lower_bound(all(header), mp(fr, j1));
			// if(j1==(*j2).ss)
			// {
			// 	cout<<"Yes"<<endl;
			// 	auto idx = j2 - header.begin();
			// 	seq.pb(mp(idx, j1));
			// }
			for(int idx = 0;idx<header.size();idx++)
			{
				if(header[idx].ff==fr && header[idx].ss==j1)
					seq.pb(mp(idx, j1));		
			}
		}
		sort(all(seq));
		for(auto j1: seq)
		{
			if((cur->child).find(j1.ss)==(cur->child).end())
			{
				struct node *temp = new node();
				temp->val = j1.ss;
				temp->freq = 1;
				cur->child[j1.ss] = temp;
				temp->parent = cur;
				if(m1.find(j1.ss)==m1.end())
				{
					vector<struct node*> temp2;
					temp2.pb(temp);
					m1[j1.ss] = temp2;
				}
				else
				{
					m1[j1.ss].pb(temp);
				}
			}
			else
			{	
				cur->child[j1.ss]->freq++;
			}
			cur = cur->child[j1.ss];
		}
		// cout<<endl;
	}
	int l1 = header.size();
	unordered_map<node*, bool> vis;
	map<ll, vector<set<ll> > >m2;
	for(auto j: m1)
		for(auto j1: j.ss)
			vis[j1] = false;
	for(int i=0;i<l1;i++)
	{
		ll last_element = header[i].ss;
		// lastt = last_element;
		s.insert(last_element);
		vector<set<ll> > v_temp;
		// inserting all the prefixes of last_element in fp-tree in v_temp;

		for(auto j: m1[last_element])
		{
			if(vis[j]==true)
			{
				continue;
			}
			else
			{
				auto temp = j->parent;
				vector<ll> s1;
				vector<node *> tra;
				while(temp->val!=-1)
				{
					s1.pb(temp->val);
					tra.pb(temp);
					temp = temp->parent;
				}
				node *tmp = j; 
				int sz = tra.size();
				for(int j1=0;j1<sz;j1++)
				{
					set<ll> up;
					if(vis[j]==true)
						continue;
					for(int j2=j1;j2<sz;j2++)
					{
						up.insert(s1[j2]);
					}
					for(int j1=0;j1<tmp->freq;j1++)
					{
						m2[tmp->val].pb(up);	
					}
					vis[tmp] = 1;
					tmp = tra[j1];
				}
			}
		}
		vector<vector<ll>> nans = fp_grow(m2[last_element], min_sup, s);
		for(auto j: nans)
		{
			j.pb(last_element);
			set<ll> f_temp;
			for(auto j1: j)
				f_temp.insert(j1);
			j.clear();
			for(auto j1: f_temp)
				j.pb(j1);
			merge.insert(j);
		}
		vector<ll> tp;
		tp.push_back(last_element);
		merge.insert(tp);
		s.erase(last_element);	
	}
	for(auto j: merge)
		ans.pb(j);
	return ans;
}

vector<vector<ll> > find_closed(vector<set<ll> > v, vector<vector<ll>> frequent, ll min_sup)
{
	auto start1 = std::chrono::system_clock::now();
	cout<<"---------------Find closed frequent itemset function is called-----------"<<endl;
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
        // //cout<<line<<endl;
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
    set<ll> s;
	cout<<"-------------Find fp-tree function is called--------------------"<<endl;
    auto start1 = std::chrono::system_clock::now();
    ans = fp_grow(v, min_sup, s);
    auto end1 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end1-start1;
	cout<<"Runtime of fp-tree function is: ";	
	cout << fixed << setprecision(4) << elapsed.count()<<endl;
    display(ans);
    cout<<endl;
    cout<<"-------------Find fp-tree-merge function is called--------------"<<endl;
    start1 = std::chrono::system_clock::now();
    ans = fp_grow_merge(v, min_sup, s);
    end1 = std::chrono::system_clock::now();
	elapsed = end1-start1;
	cout<<"Runtime of fp-tree-merge function is: ";	
	cout << fixed << setprecision(4) << elapsed.count()<<endl;
    display(ans);
    cout<<endl;
    ans = find_closed(v, ans, min_sup);
    display(ans);
    return 0;
}