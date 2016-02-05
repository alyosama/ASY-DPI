#ifndef STRINGMATCH_H
#define STRINGMATCH_H
#include <unordered_map>
#include <RawPacket.h>
#include <string>
#include <vector>
using namespace std;
class StringMatch
{
     unordered_map<int, unordered_map<char, int> > go_to;
     unordered_map<int, vector<string> > output;
     unordered_map<int,int> f;
     vector<string> keywords;
     void construct_goto();
     void construct_f();
     void enter(string y, int &new_state);
     void check(); //don't forget to delete this function
public:
    StringMatch();

     bool match(string s);
     bool matchPacket(RawPacket *rawPacket);
};

#endif // STRINGMATCH_H
