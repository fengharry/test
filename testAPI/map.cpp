#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;

  // insert
  mymap.insert ( std::pair<char,int>('a',100) );
  mymap.insert ( std::pair<char,int>('z',200) );

  // insert fail
  std::pair<std::map<char,int>::iterator,bool> ret;
  ret = mymap.insert ( std::pair<char,int>('z',500) );
  if (ret.second==false) 
  {
    std::cout << "element 'z' already existed";
    std::cout << " with a value of " << ret.first->second << '\n';
  }

  // insert position
  std::map<char,int>::iterator it = mymap.begin();
  mymap.insert (it, std::pair<char,int>('b',300));  // max efficiency inserting
  mymap.insert (it, std::pair<char,int>('c',400));  // no max efficiency inserting

  // insert range
  std::map<char,int> anothermap;
  anothermap.insert(mymap.begin(),mymap.find('c'));
  
  // find
  it = mymap.find('b');
  if (it != mymap.end())
  {
    mymap.erase (it);
  }

  // lower_bound
  std::map<char,int>::iterator itlow = mymap.lower_bound ('a');  // itlow points to a
  std::cout << "lower_bound: " << itlow->first << " => " << itlow->second << "\n";

  // uppper_bound
  std::map<char,int>::iterator itup = mymap.upper_bound ('c');  // itlow points to z
  std::cout << "upper_bound: " << itup->first << " => " << itup->second << "\n";

  // showing contents:
  std::cout << "mymap contains:\n";
  for (it=mymap.begin(); it!=mymap.end(); ++it)
  {
    std::cout << it->first << " => " << it->second << '\n';
  }

  std::cout << "anothermap contains:\n";
  for (it=anothermap.begin(); it!=anothermap.end(); ++it)
  {
    std::cout << it->first << " => " << it->second << '\n';
  }

  return 0;
}
