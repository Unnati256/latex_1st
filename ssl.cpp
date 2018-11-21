#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
//#include <stdlib.h>
//#include <cstdlib>
using namespace std;

struct node {
	string val;
	node *next = NULL;
};


int main(int argc, char** argv) {

string input = argv[1];
ifstream ques(input);
ofstream output("output.tex");

int count = 0;
string elements;
while (ques>>elements) count++;

ifstream ques1(input);
string arr[count];
for(int i = 0; i < count; i++) {
	ques1 >> arr[i];
}

// count labels
string strg ("label{");
int no = 0;
for (int i = 0; i < count; i++) {
	string t = arr[i];
	if (t.find("label{") != string::npos) {
		no++;
	}
}

// store labels in an array
string label[no];
for (int i = 0, j = 0; i < count, j < no; i++) {
	string t = arr[i];
	int n = t.length();
	if (t.find("label{") != string::npos) {
		string t2 = t.substr(7, n - 8);
		label[j] = t2;
		j++;
	}
}


// count refs
string strg2 ("ref{");
int no2 = 0;
for (int i = 0; i < count; i++) {
	string t2 = arr[i];
	if (t2.find("ref{") != string::npos) {
		no2++;
	}
}

// store refs in an array
/*string refs[no];
for (int i = 0, j = 0; i < count, j < no2; i++) {
	string t2 = arr[i];
	int n2 = t2.length();
	if (t2.find("ref{") != string::npos) {
		string t2 = t2.substr(6, n2 - 7);
		refs[j] = t2;
		j++;
	}
}*/


node *head[no];
for (int i = 0; i < no; i++) {
	head[i] = NULL;
}

for (int j = 0; j < no; j++) {
	node *temp = new node;
	temp->val = label[j];
	head[j] = temp;
	head[j]->val = temp->val;
	head[j]->next = NULL;
}

// adj list of labels.......
// second attempt

for (int i = 0; i < count; i++) {
	string bgn, end, lab, refin[no2];
	if (arr[i].find("begin{") != string::npos && arr[i] != "\\begin{document}") {
		bgn = arr[i].substr(7, arr[i].length() - 8);
		int e = i, l = i+1, c=0,r=i+1, j=0;
		while (arr[e].find("end{") != string::npos) e++;
		end = arr[e].substr(5, arr[e].length() - 6);
		if (bgn == end) {

			while (arr[l].find("label{") != string::npos && l < e) l++;
			lab = arr[l].substr(7, arr[l].length() - 8);
			while ( j < no2) {	//storing all ref inside on label in refin
				int c1 = 0;
				while (arr[r].find("ref{") != string::npos && r < e) {r++; c1=1;}
				if (c1 == 1) {
					refin[j] = arr[r].substr(6, arr[r].length() - 7);
					j++;
					r++;
				}
				if (c1 == 0) break;
			}
			for (int k = 0; k < no; k++) {
				if (lab == head[k]->val) {c = k; break;}
			}
			for ( int k = 0; k < j; k++) {
				node *t = new node;
				t->val = refin[k];
				if (head[c]->next == NULL) {
					head[c]->next = t;
					t->next = NULL;
				}
				else {
					t->next = head[c]->next;
					head[c]->next = t;
				}
			}


		}
	}
}

// first attempt
/*for (int j = 0; j < count; j++) {
	if (arr[j].find("begin{") != string::npos && arr[j] != "\\begin{document}") {
		int l = j, c, m;
		string found;
		int k = j,y;
		while (arr[k].find("end{") != string::npos) {
			while (arr[l].find("label{") != string::npos && l < count) {
				l++;
			}
			found = arr[l].substr(7, arr[l].length() - 8);
			for (int w = 0; w < no; w++) {
				if (found == head[w]->val) {c = w; break;cout <<" head "<< head[c]->val<< endl;}
			}
			while (y < count) {
				if (arr[y].find("ref{") != string::npos) {
					node *t = new node;
					t->val = arr[y].substr(6, arr[y].length() - 7);
					if (head[c]->next == NULL) {
						head[c]->next = t;
						t->next = NULL;
					}
					else {
						t->next = head[c]->next;
						head[c]->next = t;
					}
				}
				y++;
			}
			k++;
		}
	}
}*/	


node *temp = new node;
temp->val = "eq3";
head[0]->next = temp;
node *temp1 = new node;
temp1->val = "eq5";
head[0]->next->next = temp1;
node *temp2 = new node;
temp2->val = "eq1";
head[7]->next = temp2;

for (int i = 0; i < no; i++) {
	node *qq = new node;
	qq = head[i]->next;
	cout << head[i]->val << " - ";
	while (qq != NULL) {
		cout << qq->val << " ";
		qq = qq->next;
	}
	cout << endl;
}





output << "\\documentclass{article}"<< "\n"<< "\\usepackage{tikz}" <<"\n" <<"\\usetikzlibrary{arrows,shapes}" <<"\n" <<"\\begin{document}" << "\n" << "\\begin{tikzpicture}[scale=2.5]"<< "\n" <<"\\tikzstyle{every node}=[draw, shape = circle];" << "\n"<<"\n";

output << "\\node (" << head[0]->val << ") at(0,0) {" << head[0]->val << "};" << "\n";
for (int i = 1; i < no; i++) {
	output << "\\path(" << (i*360)/(no-1) << ":2cm) node (" << head[i]->val << ") {" << head[i]->val << "};" << "\n";
}

for(int i = 0; i < no; i++) {
	if (head[i]->next != NULL) {
		node *f = new node;
		f = head[i]->next;
		while (f != NULL) {
			output << "\\draw [->] (" << head[i]->val << ") -- (" << f->val << ");" << "\n";
			f = f->next;
		}
	}
}



output << "\\end{tikzpicture}"<< "\n" <<"\\end{document}";

//compiling using pdflatex in terminal

//string pdf = "pdflatex ";
//pdf = pdf + "output.tex";
//const char *command = pdf.c_str();
//system("pdflatex output.tex");



}

