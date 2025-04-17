#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

// Function to split a string into tokens
vector<string> split(const string &s) {
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);
  while (tokenStream >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

// Main UCI loop
void uci_loop() {
  string line;
  vector<string> tokens;

  while (getline(cin, line)) {
    tokens = split(line);

    if (tokens.empty())
      continue;

    if (tokens[0] == "uci") {
      // Identify the engine
      cout << "id name ChessEngine" << endl;
      cout << "id author Hardcode" << endl;
      // Send options available
      cout << "uciok" << endl;
    } else if (tokens[0] == "isready") {
      // Engine is ready
      cout << "readyok" << endl;
    } else if (tokens[0] == "ucinewgame") {
      // Reset the engine for a new game
      // TODO: Implement new game initialization
    } else if (tokens[0] == "position") {
      // Handle position command
      // TODO: Implement position parsing and setup
    } else if (tokens[0] == "go") {
      // Start calculating
      // TODO: Implement move calculation
      cout << "bestmove e2e4" << endl; // Placeholder
    } else if (tokens[0] == "quit") {
      // Exit the program
      break;
    } else if (tokens[0] == "stop") {
      // Stop calculating
      // TODO: Implement stop calculation
    }
  }
}

int main() {
  uci_loop();
  return 0;
}
