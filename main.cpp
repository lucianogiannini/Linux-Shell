#include <iostream>
#include <main.h>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

extern int errno;
using namespace std;

int main(void)
{
  bool redirect;
  bool pipess;
  string theLine;
  int argc;
  char* argv[100];
  char* argvnew[100];
  while(true)
    {
  for(int i = 0; i < 100; i++)
    {
      argv[i] = 0;
      argvnew[i] = 0;
    }
  theLine = "";
  cout << "Bash O' Rama>";
  getline(cin,theLine);
  string theLine2 = theLine;
  cout << "Command was: " << theLine << endl;
  if((theLine == "exit") || cin.eof())
    {
      exit(0);
    }
  string delimiter = " ";
  size_t pos = 0;
  string token;
  bool andop = false;
  int counter = 0;
  int counter1 = 0;
  int redirectsless [2] = {0,0};
  int redirectsgreater [2] = {0,0};
  int redirectscountergreater = 0;
  int redirectscounterless = 0;
  int pipearray [2] = {0,0};
  int pipecounter = 0;
  redirect = false;
  pipess = false;
  const string arrow1 = ">";
  const string arrow2 = "<";
  while((pos = theLine.find(delimiter)) != string::npos)
    {
      token = theLine.substr(0,pos);
      // int p = token.find("./");
      // cout << "]"<<token<<"[" << endl;
      theLine.erase(0,pos + delimiter.length());
      if(token != ">" && token != "<" && token != "|")
	{
	  if(pipess == true)
	    {
	      char *temp = new char[token.length() + 1];
	      strcpy(temp,token.c_str());
	      argvnew[counter1] = temp;
	      counter1 = counter1 + 1;
	    }
	  else
	    {
	      char *temp = new char[token.length() + 1];
	      strcpy(temp,token.c_str());
	      argv[counter] = temp;
	      counter = counter + 1;
	    }
	}
      else
	{
	  if(token == "|")
	    {
	      pipearray[pipecounter] = counter;
	      pipecounter++;
	      pipess = true;
	    }
	  if(token  == ">" || token == "<")
	    {
	      redirect = true;
	      if(token == ">")
		{
		  redirectsgreater[redirectscountergreater] = counter;
		  redirectscountergreater++;
		}
	      if(token == "<")
		{
		  redirectsless[redirectscounterless] = counter;
		  redirectscounterless++;
		}
	    }
	}
      //cout << redirect << endl;
    }
  //  cout << "pipe boolean " << pipess << endl;
  //cout << "redirect boolean " << redirect << endl;
  if(theLine == "&")
    {
      andop = true;
      argv[counter] = NULL;
    }
  else//not &
    {
      //cout << theLine << endl;
      if(pipess == false)
	{
	  andop = false; 
	  char *temp = new char[theLine.length() + 1];
	  strcpy(temp,theLine.c_str());
	  argv[counter] = temp;
	  argv[counter+1] = NULL;
	}
      else
	{
	  andop = false; 
	  char *temp = new char[theLine.length() + 1];
	  strcpy(temp,theLine.c_str());
	  argv[counter] = NULL;
	  argvnew[counter1] = temp;
	  argvnew[counter1+1] = NULL;
	}
    }
  ifstream in;
  in.open("/etc/environment");
  string a = " ";
  in>>a;
  string paths[8];
  a.erase(0,6);//gets rid of path="
  int count = 1;
  do
    {
      int num = a.find(":");//find first :
      paths[count] = a.substr(0,num);//saves the first path name
      //cout << paths[count] << endl;//prints path to check
      a.erase(0,num + 1);//gets rid of path
      // cout << a << endl;//prints result
      count++;
    }while(count!=7);
  int numq = a.find('"');//find first :
  paths[count] = a.substr(0,numq);//saves the first path name
  //cout << paths[count] << endl;//prints path to check
  a.erase(0,numq + 1);//gets rid of path
  //cout << a << endl;//prints result
  
  string realPath = "";
  bool found = false;
  //cout << paths[0] << endl;
  
  for(int i = 0; i < 8; i++)
    {
      
      string s = paths[i]+"/"+argv[0];
      //cout << s << endl;
      ifstream ifs(s);
      if(! ifs.is_open())
	{
	  //cout << "Failed to open" << endl;
	}
      else
	{
	  //cout << "Opened OK" << endl;
	  realPath = s;
	  found = true;
	  break;
	}
      s = argv[0];
      ifstream ifss(s);
      
      if(! ifss.is_open())
	{
	  //cout << "Failed to open" << endl;
	}
      else
	{
	  //cout << "Opened OK" << endl;
	  realPath = s;
	  found = true;
	  break;
	}
      
      
    }
  if(found == true && realPath != "/")
    {
      //cout << realPath << endl;
      //cout << "andop: " << andop << endl;
      const char* path = realPath.c_str();
      if(pipess == false)
	{
	  int newstdin = 0;
	  int newstdout = 1;
	  if(fork() == 0)
	    {
	      newstdin = 0;
	      newstdout = 1;
	      
	      if(redirect == true)
		{
		  if(redirectsgreater[0] != 0 && redirectsless[0] != 0)
		    {
		      //cout << "in first if statement" << endl;
		      int first = redirectsgreater[0];
		      newstdin = open(argv[first-1],O_RDONLY);
		      close(0);
		      dup(newstdin);
		      newstdout = open(argv[first],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
		      close(1);
		      dup(newstdout);
		      argv[first] = NULL;
		      argv[first-1] = NULL;
		    }
		  else if(redirectsgreater[0] != 0)
		    {
		      //cout << "path" << path << endl;
	     
		      //cout << "in second if statement" << endl;
		      int first = redirectsgreater[0];
		      //cout << "first" << first << endl;
		      newstdout = open(argv[first],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
		      //cout << "argv[first]" << argv[first] << endl;
		      close(1);
		      dup(newstdout);
		      argv[first] = NULL;
		    }
		  else if(redirectsless[0] != 0)
		    {
		      //cout << "in third if statement" << endl;
		      int first = redirectsless[0];
		      newstdin = open(argv[first],O_RDONLY);
		      close(0);
		      dup(newstdin);
		      argv[first] = NULL;
		    }
		}
	      execvp(path,argv);
	    }
	  else//no redirect > <
	    {
	      int status = 0;
	      if(andop == false)
		{
		  wait(&status);
		  cout << "Child exited with status of " << status << endl;
		  if(redirect == true)
		    {
		      //close(newstdin);
		      //close(newstdout);
		    }
		}
	      else
		{
		  //cout << getpid() << endl;
		}
	    }
	  
	}
      else//pipess is true
	{
	  int pid;
	  
	  int thePipe[2];
	  
	  pid = fork();
	  if(pid != 0)//parent
	    {
	      int status = 0;
	      
	      wait(&status);
	      cout << "Child exited with status of " << status << endl;
	    }  
	  
	  else if(pid ==0)//child
	    {
	      pipe(thePipe);
	      
	      int pid2;
	      pid2 = fork();
	      if(pid2 == 0)
		{
		  close(thePipe[0]);//close read
		  
		  int first = pipearray[0];
		  //newstdout = open(argv[first],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
		  close(1);
		  dup(thePipe[1]);//write to stdout
		  close(thePipe[1]);
		  //argv[first] = NULL;
		  //argv[first] = NULL;
		  
		  //path has cat
		  //argv should have out.txt
		  execvp(path,argv);
		  
		  
		}
	      else
		{
		  int first = pipearray[0];
		  
		  close(thePipe[1]);//close write
		  close(0);
		  dup(thePipe[0]);//read to stdin
		  close(thePipe[0]);
		  //path needs to have wc
		  //argv needs the arguments of wc which is -l
		  
	  	  
		  
		  //	    cout << "finding new path" << endl;
		  /////////////////////
		  //we need to find the new path now of argvnew[0]
		  /////////////////////////
		  string realPath = "";
		  bool found = false;
		  //cout << paths[0] << endl;
		  
		  for(int i = 0; i < 8; i++)
		    {
		      
		      string s = paths[i]+"/"+argvnew[0];
		      //  	  cout << s << endl;
		      ifstream ifs(s);
		      if(! ifs.is_open())
			{
			  //cout << "Failed to open" << endl;
			}
		      else
			{
			  //cout << "Opened OK" << endl;
			  realPath = s;
			  found = true;
			  break;
			}
		      s = argvnew[0];
		      ifstream ifss(s);
		      
		      if(! ifss.is_open())
			{
			  //cout << "Failed to open" << endl;
			}
		      else
			{
			  //cout << "Opened OK" << endl;
			  realPath = s;
			  found = true;
			  break;
			}
		      
		      
		    }//end for loop for finding the path
		  if(found == true && realPath != "/")
		    {
		      path = realPath.c_str();
		      execvp(path,argvnew);
		      
		    }
		  
		  
		  
		}
	      
	    }
	}//pipess is true {}
    }
    }
  
  return 1;
}
