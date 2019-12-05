#include <iostream>         // for the input/output
#include <stdlib.h>         // for the getenv call
#include <sys/sysinfo.h>    // for the system uptime call
#include <cgicc/Cgicc.h>    // the cgicc headers
#include <cgicc/CgiDefs.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <unistd.h>
#define LED_GPIO "/sys/class/gpio/gpio60/"
using namespace std;
using namespace cgicc;

void writeGPIO(string filename, string value){
   fstream fs;
   string path(LED_GPIO);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

int main(){
   Cgicc form;                      // the CGI form object
   string cmd;                      // the Set LED command
   string blinks;                   // how many times the LED will be blinked
   writeGPIO("direction", "out");
   // get the state of the form that was submitted - script calls itself
   form_iterator it = form.getElement("numbers");  // amount of blinks
   if (it == form.getElements().end() || it -> getValue()=="")
         {
          blinks = "2";
         }
    else
         {
          blinks == it->getValue();
         }
   it = form.getElement("cmd");  //get radio command
   if (it == form.getElements().end() || it->getValue()==""){
      cmd = "off";                     // if it is invalid use "off"
   }
   else { cmd = it->getValue(); }      // otherwise use submitted value


   char *value = getenv("REMOTE_ADDR");    // The remote IP address
 
   // generate the form but use states that are set in the submitted form
   cout << HTTPHTMLHeader() << endl;       // Generate the HTML form
   cout << html() << head() << title("LED Example") << head() << endl;
   cout << body() << h1("Exploring BB POST LED Example") << endl;;
   cout << "<form action=\"/cgi-bin/led.cgi\" method=\"POST\">\n";
   cout << "<div>Set LED: <input type=\"radio\" name=\"cmd\" value=\"on\""
        << ( cmd=="on" ? "checked":"") << "/> On ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"off\""
        << ( cmd=="off" ? "checked":"") << "/> Off ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"blink\""
	<< ( cmd=="blink" ? "checked":"") << "/> Blink ";
   cout << "<div>Number of Blinks: <input type=\" text\" name=\"numbers\" value=\""
	<< blinks <<"\"> times      ";
   cout << "<input type=\"submit\" value=\"Set LED\" />";
   cout << "</div></form>";

int blink = stoi(blinks);  // converting value of blinks from a string to an int
 
   // process the form data to change the LED state
   if (cmd=="on") writeGPIO("value", "1");              // turn on
   else if (cmd=="off") writeGPIO("value", "0");        // turn off
   else if (cmd=="blink")
	{
	 for(int i = 0; i < blink; i++)         //blink loop
		{
		 writeGPIO("value", "1");
		 sleep(1);
		 writeGPIO("value", "0");
		 sleep(1);
		}
	}
   else cout << "<div> Invalid command! </div>";        // not possible
   cout << "<div> The CGI REMOTE_ADDR value is " << value << "</div>";
   cout << body() << html();
   return 0;
}
