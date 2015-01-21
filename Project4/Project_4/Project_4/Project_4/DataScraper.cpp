/******************************************************************************
Title:       University and College Data Analysis/Webscraping      
 Programmer: Samantha Armstrong     
 Course:     CSCI 240     
 Instructor: Prof. Ringenberg     
 Due Date:   November 6, 2014     

 Description: 
	This program calculates the total number of enrolled students in Ohio 
	colleges, the average number of students enrolled in Ohio colleges, and 
	the average college tuition in Ohio. This program also allows the user to 
	search for colleges based on a partial search at the beginning of the 
	name of the college.
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
using namespace std;

//should all of these be global?
//how do I make these fit in the column max while still making it look nice?
string	getPageContents(string fileName);
void	scrapeDataIntoArray(string webpage, string begTag, string endTag, string arr[],
							int &numSchools, int MAX);
void	scrapeDataIntoArray(string webpage, string begTag, string endTag, int arr[],
							int &numSchools, int MAX);
int		stripCommasFromNum(string temp);
void	writeSchools(string name[], string loc[], int tuition[], int enrollment[], 
					 string years[], string type[], int &numSchools, string fileName);
void	computeAvgs(int enrollment[], int tuition[], int &numSchools);
void	searchByName(string name[], string loc[], int tuition[], int enrollment[],
					 string years[], string type[], int &numSchools, string searchName);

int main(){
	const int MAX = 1000;

	int numSchools = 0;

	string webpage;
	string begTag;
	string endTag;
	
	string    name[MAX];		//school name array
	string    loc[MAX];			//location
	int       tuition[MAX];		//tuition		
	int		  enrollment[MAX];  //enrollment
	string    years[MAX];		//year (2 or 4)
	string    type[MAX];	    //type (Public/private)

	string	  fileName = "scraped.csv";			//csv file name

	webpage = getPageContents("Source.htm");

	string searchName;	//full or partial name of school to search for
	string END = "#";

	//Scrape School names into name array
	begTag = "<h4>";
	endTag = "</h4>";
	scrapeDataIntoArray(webpage, begTag, endTag, name, numSchools, MAX);

	//Scrape school location into loc array
	begTag = "<small>";
	endTag = "</small>";
	scrapeDataIntoArray(webpage, begTag, endTag, loc, numSchools, MAX);

	//Scrape data into tuition array
	begTag = "<td class=\"tuition\">";
	endTag = "</td>";
	scrapeDataIntoArray(webpage, begTag, endTag, tuition, numSchools, MAX);

	//Scrape data into enrollment array
	begTag = "<td class=\"size\">";
	endTag = "</td>";
	scrapeDataIntoArray(webpage, begTag, endTag, enrollment, numSchools, MAX);

	//Scrape data into years array
	begTag = "<td class=\"years\">";
	endTag = "</td>";
	scrapeDataIntoArray(webpage, begTag, endTag, years, numSchools, MAX);

	//Scrape data into type array
	begTag = "<td class=\"type\">";
	endTag = "</td>";
	scrapeDataIntoArray(webpage, begTag, endTag, type, numSchools, MAX);
	

	writeSchools(name, loc, tuition, enrollment, years, type, numSchools, fileName);

	computeAvgs(enrollment, tuition, numSchools);

	while(searchName != END){
		cout << "\nPlease enter a school name to search for (# to stop): ";
		getline(cin, searchName);
    
		if(searchName != END){
			searchByName(name, loc, tuition, enrollment, years, type, numSchools, searchName);
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//Get the contents of an input file and store into a string
//variable to be returned
///////////////////////////////////////////////////////////////////////////////
string getPageContents(string fileName){
	ifstream f(fileName);
	string str;

	f.seekg(0, ios::end);   
	str.reserve(f.tellg());
	f.seekg(0, ios::beg);

	str.assign((istreambuf_iterator<char>(f)),
		istreambuf_iterator<char>());

	f.close();

	return str;
}

void scrapeDataIntoArray(string webpage, string begTag, string endTag, string arr[], int &numSchools, int MAX){
	int beg = 0;
	int end = 0;
	int len;
	numSchools = 0;
	//find the first instance
	beg = webpage.find(begTag, beg);

	while(beg != -1){
		//move the beg index to after the end of the tag and find end index
		beg += begTag.length();
		end = webpage.find(endTag, beg);

		//compute the length of the substring based on the beg and end indeces
		len = end - beg;

          //output demonstration (don't keep, but can be used for testing)
		// cout << webpage.substr(beg, len) << endl;

	
		arr[numSchools] = webpage.substr(beg, len);
		numSchools++;
	
		//move beg to end of the ending tag and find the next begin tag
		beg = end + endTag.length();
		beg = webpage.find(begTag, beg);

	}//end while

}//end scrapeDataIntoArray String version

void scrapeDataIntoArray(string webpage, string begTag, string endTag, int arr[], int &numSchools, int MAX){
	int beg = 0;
	int end = 0;
	int len;
	string temp;
	int num;
	numSchools = 0;
	//find the first instance
	beg = webpage.find(begTag, beg);

	while(beg != -1){
		//move the beg index to after the end of the tag and find end index
		beg += begTag.length();
		end = webpage.find(endTag, beg);

		//compute the length of the substring based on the beg and end indeces
		len = end - beg;
 	
		temp = webpage.substr(beg, len);

		//call the strip commas from num function
		num = stripCommasFromNum(temp);

		arr[numSchools] = num;
		numSchools++;
	
		//move beg to end of the ending tag and find the next begin tag
		beg = end + endTag.length();
		beg = webpage.find(begTag, beg);

	}//end while

}//end scrapeDataIntoArray Integer version

int stripCommasFromNum(string temp){
	int number;

	if (temp[0] == '$'){
		temp.erase(0,1);
	}//end if
	for(int i = temp.length() - 1; i >= 0; i--){
		if (temp[i] == ','){
			temp.erase(i,1);
		}//end if
	}//end for

	number = stoi(temp.c_str());

	return number;

}//end stripCommasFromNum

void writeSchools(string name[], string loc[], int tuition[], int enrollment[], 
				  string years[], string type[], int &numSchools, string fileName){
	ofstream outFile(fileName);

	//error message for input file
	if (!outFile){
		cout << "Could not open file." << endl;
	} //end if error
	
	for(int n = 0; n < numSchools; n++){
		outFile << name[n]			<< ",";
		outFile << loc[n]			<< ",";
		outFile << tuition[n]		<< ",";
		outFile << enrollment[n]	<< ",";
		outFile << years[n]			<< ",";
		outFile << type[n]			<< "," << endl;
	}//end for

	//Close the input file
	outFile.close();

}//end writeSchools

void computeAvgs(int enrollment[], int tuition[], int &numSchools){
	int totalEnroll = 0;
	int totalTuition = 0;
	double averageEnroll;
	double averageTuition;

	for(int z = 0; z < numSchools; z++){
		totalEnroll += enrollment[z];
		totalTuition += tuition[z];
	
	}//end for

	cout << "The total number of enrolled students in Ohio colleges is: " 
		 << totalEnroll << endl;

	averageEnroll = totalEnroll / numSchools;
	cout << "The average number of enrolled students in Ohio colleges is: " 
		 << setprecision(0) << fixed << averageEnroll << endl;

	averageTuition = totalTuition / numSchools;
	cout << "The average college tuition in Ohio is: $" 
		 << setprecision(0) << fixed << averageTuition << endl;

}//end computeAvgs

void searchByName(string name[], string loc[], int tuition[], int enrollment[], 
				  string years[], string type[], int &numSchools, string searchName){
	bool found = false;

	for(int j = 0; j < numSchools; j++){
		if (name[j].find(searchName) == 0){
			found = true;
			
			cout << left << setw(12) << "Name:       " << name[j]		<< endl;
			cout << left << setw(12) << "Location:   " << loc[j]		<< endl;
			cout << left << setw(12) << "Tuition:    " << tuition[j]	<< endl;
			cout << left << setw(12) << "Enrollment: " << enrollment[j] << endl;
			cout << left << setw(12) << "Type:       " << type[j]		<< endl;
			cout << left << setw(12) << "Years:      " << years[j]		<< endl;
			cout << endl;
		
		}//end if

	}//end for
	
	if (found == false){
			cout << "No results found" << endl;
	}//end if 
	
}//end searchByName