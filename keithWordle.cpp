#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <fstream>
#include <vector>

using namespace std;

//node struct
struct Node{
    char letter;
    Node* next;
};

//Wordle class
class Wordle{
public:
    //constructor
    Wordle():attempts{0}, head{nullptr}, positions{' '}{};
    //destructor
    ~Wordle();
    //generate random word
    string loadWord();
    //full game
    void startGame();
private:
    Node* head;
    Node* tail;
    string word;
    int attempts;
    char positions[5];
};

Wordle::~Wordle(){
    Node* current = head;
    while(current != nullptr){
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

string Wordle::loadWord(){
    srand(time(NULL));
    int size = 0;
    
    vector<string> list;
    ifstream iFile;
    iFile.open("wordBank.txt");
    if(!iFile.is_open()){
        cout << "File not found!" << endl;
    }
    else{
        while(getline(iFile, word)){
            size++;
            list.push_back(word);
        }
    }
    iFile.close();

    int randIndex = rand() % size;
    return list[randIndex];
    


}

void Wordle::startGame(){
    cout << "Welcome to Wordle!" << endl;
    cout << "Loading Word..." << endl;
    unordered_map<char, int> letterCount;
    unordered_map<char, int> copyHash;

    string word = loadWord();
    for(int i = 0; i < word.length(); i++){
        positions[i] = word[i];
        letterCount[word[i]]++;
        copyHash[word[i]]++;
        Node* newNode = new Node;
        newNode->letter = word[i];
        if(head == nullptr){
            newNode->next = nullptr;
            head = newNode;
            tail = newNode;
        }
        else{
            newNode->next = nullptr;
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    while (true) {
        string guess = "";
        cout << "Enter Guess or 'q' to quit playing" << endl;
        cin >> guess;
        if(guess == "q"){
            break;
        }

        while(guess.length() != 5){
            cout << "Please enter a 5-letter guess!" << endl;
            cin >> guess;
        }
        attempts++;

        if(guess == word){
            cout << "\033[32m" << guess << "\033[0m" << endl;
            cout << "You won! The word was " << word << " within " << attempts << " tries" << endl;
            break; 
        }

        Node* current = head;

        for(int i = 0; i < guess.length(); i++){
            if(current != nullptr){
                if(guess[i] == current->letter && letterCount[guess[i]] != 0){
                    letterCount[guess[i]]--;
                    //green
                    cout << "\033[32m" << guess[i] << "\033[0m" << endl;
                }
                else if (word.find(guess[i]) != std::string::npos && letterCount[guess[i]] != 0) {
                    letterCount[guess[i]]--;
                    //yellow
                    cout << "\033[33m" << guess[i] << "\033[0m" << endl;
                }
                else{
                    cout << guess[i] << endl;
                }
                current = current->next;
            }
        }
        letterCount = copyHash;
    }
}

int main(){
    Wordle newGame;
    newGame.startGame();
    return 0;
}