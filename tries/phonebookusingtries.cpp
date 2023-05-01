#include <iostream>
#include <cstring>
#include <string>

using namespace std;

template<typename T>    //generic program for data multiple types 
class Node{     //node to be used in the hash table
public:
    string key;
    T value;
    Node<T>*next;
    Node(string key,T val) {    //constructor
        this->key = key;
        value = val;
        next = NULL;
    }
    ~Node(){    //destructor
        if(next !=NULL) {   //this ensures that there is no memory leak
            delete next;
        }
    }
};

template<typename T>
class Hashtable{    //the class of hashtable
    Node<T>** table; //pointer to an array of pointers
    int current_size;
    int table_size;
    int hashFn(string key){ //the main hash function, it uses the seperate chaining methodology
        int idx = 0;
        int p = 1;
        for(int j=0; j<key.length(); j++){
            idx = idx + (key[j] *p)%table_size;
            idx = idx%table_size;
            p = (p*27)%table_size;
        }
        return idx;
    }

    void rehash(){  //rehash function that is invoked when the load factor increases and crosses 0.7
        Node<T>** oldTable = table;
        int oldTableSize = table_size;
        table_size = 2*table_size; //approximation for the next prime number
        table = new Node<T>*[table_size];
        for(int i=0;i<table_size;i++){      //initialize the entire hash table with NULL
            table[i] = NULL;
        }

        current_size = 0;
        for(int i=0; i<oldTableSize; i++) {     //copy data to the new table
            Node<T> *temp = oldTable[i];
            while (temp !=NULL) {
                insert(temp->key, temp->value);
                temp = temp -> next;
            }
            if(oldTable [i] !=NULL){
                delete oldTable[i];
            }
        }
        delete [] oldTable;
    }

public:
    Hashtable(int ts=7){    //hashtable constructor, initial size is 7 but rehashing 
                            //may be done if load factor increases
        table_size = ts;
        table = new Node<T>*[table_size];
        current_size = 0;
        for(int i=0; i<table_size; i++) {
            table[i] = NULL;
        }
    }
    void insert(string key, T value) {
        int idx = hashFn(key);
        Node<T>*n = new Node<T>(key, value); ;
        //Insert at head of the linked list with id=idx
        n->next = table[idx];
        table[idx] = n;
        current_size++;
        float load = current_size/(1.0*table_size); 
        //rehash the table if necessary
        //condition checked every time insertion is done
        if(load>0.7)
            rehash();
    }

    void print() { 
        //print the entire hash table
        for(int i = 0; i < table_size; i++) { 
            cout << "Bucket " << i << " ->"; 
            Node<T>*temp = table[i]; 
            while(temp != NULL) { 
                cout << temp->key << "->"; 
                temp = temp->next; 
            }
        }
    }

    T* search(string key) { 
        //search function to check for the presence and absence of an element
        int idx = hashFn(key); 
        Node<T>*temp = table[idx]; 
        while(temp != NULL) { 
            if (temp->key == key){ 
                return &temp->value; 
            } 
            temp = temp->next; 
        } 
        return NULL; 
    } 
    T& operator[] (string key){ 
        //square brackets overloaded operator
        T *f = search(key); 
        if(f == NULL){ 
            T garbage; 
            insert(key,garbage); 
            f = search(key); 
        } 
        return *f;
    } 
};

Hashtable<long long int> contacts;         // A hash table to store the key value pair of the contacts with the name 
struct trie {									// Defining a structure for the trie nodes
    struct trie * ch[26];
    bool islast;
};
// This is used to define a root for trie here we define the 26 letter nodes as NULL and islast for all of them as false
struct trie * getnode(void) {			
    struct trie * p = new trie;
    for (int i = 0; i < 26; i++)
        p -> ch[i] = NULL;
    p -> islast = false;
    return p;
}
struct trie * root = getnode();					// Defining the root node

void insert(string key) {						// Function for insertion in a trie	
    struct trie * temp = root;
    for (int i = 0; i < key.length(); i++) {

		// Ascii value of the individual letters are subtracted from 'a' so as to find the index value for trie
		int id = key[i] - 'a';					

		// If the node is empty at that index meaning no word with that certain letter has been entered
        if (temp -> ch[id] == NULL)				
            temp -> ch[id] = getnode();
        temp = temp -> ch[id];
    }
	// When the word gets finished we change the value of islast to true meaning the word has ended
    temp -> islast = true;						
}

// It is function to find all the suggestion with given prefix. It is a recursive function
void sug(struct trie * word, string prefix) {

/* The base case for this recursive function is to check whether the islast of that node is true or not 
	if it is true then it prints contact from the given map contact corresponding to that prefix */
    if (word -> islast == true)
        cout << prefix << "  " << contacts[prefix] << "\n";

	/* Here we traverse further on the word node, we find all the suggestions for "word" or "prefix" by checking 
		whether there are any children node for word node */
    for (int i = 'a'; i <= 'z'; i++) {

		// We check for all 26 letters of the alphabet by using the above for loop this will form a recursion tree
        struct trie * next = word -> ch[i - 'a'];
        if (next != NULL)

			// Here prefix is updated if a string is with that prefix is found and the new prefix is passed in the recursion
            sug(next, prefix + (char) i);
    }
}

void displaycontacts(string str) {
    // A string is used to compare with the strings entered in the directory
    string prefix = "";
    int i;
    struct trie * prev = root;
    for (i = 0; i < str.length(); i++) {
        prefix += str[i];

		// Ascii value of the individual letters are subtracted from 'a' so as to find the index value for trie
        int id = prefix[i] - 'a';

		// Here it is checked whether the character at the above calculated index is present. If it is not present then return "Not Found"
        struct trie * current = prev -> ch[id];
        if (current == NULL) {
            i++;
            cout << "Not found ";
            break;
        }

		// If we have traversed the whole string then we check the suggestion by passing the prefix to sug function
        if (i == str.length() - 1) {
            cout << "suggestions of " << prefix << " are\n";
            sug(current, prefix);
        }
        prev = current;

    }

    for (; i < str.length(); i++) {
        prefix += (char) str[i];
        cout << "No Results Found for " << prefix << "\n";
    }
}

int main() {
    while(1){
        cout<<"1. Insert Contact";
        cout<<"\n2. Search Contact";
        cout<<"\n3. Exit";
        cout<<"\nEnter a choice:";
        int choice;
        cin>>choice;
        string name, search;
        long long int phno;
        switch (choice){
            case 1: 
                    cout << "Enter the contact's Name - ";
                    cin >> name;
                    cout << "\nEnter the phone number of the contact - ";
                    cin >> phno;
                    insert(name);
                    // Entering details in the map
                    contacts[name] = phno;
                    break;

            case 2:
                    cout << "\nName or Prefix of the contact's name you want to search - ";
                    cin >> search;
                    displaycontacts(search);
                    break;
            case 3:
                    exit(0);
        
            default:
                    cout<<"\nInvalid choice!";
        }
    }
    return 0;
}
