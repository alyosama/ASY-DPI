#include "stringmatch.h"
#include <queue>
#include <iostream>
#include <set>

//implementation of aho-corasick algorithm of string matching to match signatures with packets payload



//enter and construct_goto generate the goto function and part of the output function
void StringMatch::enter(string y, int &new_state)
{
    int state=0, j=0, l=y.size();
    while(go_to[state].find(y[j]) != go_to[state].end() && j<l)
    {
        state = go_to[state][y[j]];
        j++;
    }
    for(int p=j;p<l;p++)
    {
        new_state++;
        go_to[state].insert(make_pair(y[p],new_state));
        state = new_state;
    }
    output[state].push_back(y);
}

void StringMatch::construct_goto()
{
    int new_state=0, k=keywords.size();
    for(int i=0;i<k;i++)    enter(keywords[i], new_state);
}

//this function generate the failure function f and the rest of output function
void StringMatch::construct_f()
{
    queue<int> q;
    unordered_map<char, int>::iterator it = go_to[0].begin();
    unordered_map<char, int>::iterator it1 = go_to[0].end();
    for(;it!=it1;it++)
    {
        if(it->second>0)
        {
            q.push(it->second);
            f[it->second]=0;
        }
    }
    while (!q.empty())
    {
        int r = q.front();
        q.pop();
        it = go_to[r].begin();
        it1 = go_to[r].end();
        for(;it!=it1;it++)
        {
            q.push(it->second);

            int state= f[r];

            while(go_to[state].find(it->first) == go_to[state].end() && state != 0)   state = f[state];

            if(go_to[state].find(it->first) != go_to[state].end())
                f[it->second] = go_to[state][it->first];
            else
                f[it->second] = 0;
            for(int i=0;i < output[f[it->second]].size(); i++)
                output[it->second].push_back(output[f[it->second]][i]);

        }
    }
}

//constructor
StringMatch::StringMatch()
{
    //keywords should be read from file or something
    keywords.push_back("he");
    keywords.push_back("she");
    keywords.push_back("his");
    keywords.push_back("hers");
    keywords.push_back("entertain");
    this->construct_goto();
    this->construct_f();
   // this->check();
}

//match a string withe keywords
//note that it can be edited to return the output index to determine which keyword matched the input string
bool StringMatch::match(string a)
{
    int state = 0, n=a.length();
    for(int i=0;i<n;i++)
    {
        while(go_to[state].find(a[i])==go_to[state].end() && state!=0) state = f[state];
        state = go_to[state][a[i]];
        if(output.find(state) != output.end() && output[state].size() > 0)
            return true;
    }
    return false;
}

//converts the packet data into a string and pass it to the matching function
bool StringMatch::matchPacket(RawPacket *rawPacket)
{

    string s ;
    unsigned const char* sx = rawPacket->getRawData();
    int l = rawPacket->getRawDataLen();
    for(int i=0;i<l;i++)
        s += sx[i];
    //cout<<sss<<endl<<endl;
    //cout<< reinterpret_cast<const char*>(rawPacket->getRawData())<<endl<< sss << endl << sss.size()<<endl<<rawPacket->getRawDataReadOnly()<<endl<<endl;
    return this->match(s);
}

//prints the table for testing
void StringMatch::check()
{
    cout<<endl<<"goto:"<<endl;
    for(int i=0;i<10;i++)
    {
        unordered_map<char, int>::iterator it = go_to[i].begin();
        unordered_map<char, int>::iterator it1 = go_to[i].end();
        for(;it!=it1;it++)
        {
           cout<<it->first <<" "<<it->second<<endl;
        }
    }
    cout<<endl<<"failure function"<<endl;
    unordered_map<int, int>::iterator itt = f.begin();
    unordered_map<int, int>::iterator itt1 = f.end();
    for(;itt!=itt1;itt++)
        cout<<itt->first <<" "<<itt->second<<endl;

    cout<<"output:";
    unordered_map<int, vector<string> >::iterator iv=output.begin();
    unordered_map<int, vector<string> >::iterator iv1=output.end();
    for(;iv!=iv1;iv++)
    {
        int l=iv->second.size();
        cout<<endl<<iv->first<<" ";
        for(int i=0;i<l;i++)
            cout<<iv->second[i]<<" ";
    }
    cout<<endl<<"keywords:"<<endl;

    for(int i=0;i<keywords.size();i++)
        cout<<keywords[i]<<endl;

}
