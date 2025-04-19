#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istream;
using std::istringstream;
using std::ostream;
using std::string;
using std::vector;

// Function to split a string into tokens
vector<string>
split ( const string &s )
{
  vector<string> tokens;
  string         token;
  istringstream  tokenStream ( s );
  while ( tokenStream >> token )
  {
    tokens.push_back ( token );
  }
  return tokens;
}

// Main UCI loop
void
uci_loop ( istream &input = cin, ostream &output = cout )
{
  string         line;
  vector<string> tokens;

  while ( getline ( input, line ) )
  {
    tokens = split ( line );

    if ( tokens.empty () )
      continue;

    if ( tokens[0] == "uci" )
    {
      // Identify the engine
      output << "id name ChessEngine" << endl;
      output << "id author Hardcode" << endl;
      // Send options available
      output << "uciok" << endl;
    }
    else if ( tokens[0] == "isready" )
    {
      // Engine is ready
      output << "readyok" << endl;
    }
    else if ( tokens[0] == "ucinewgame" )
    {
      // Reset the engine for a new game
      // TODO: Implement new game initialization
    }
    else if ( tokens[0] == "position" )
    {
      // Handle position command
      // TODO: Implement position parsing and setup
    }
    else if ( tokens[0] == "go" )
    {
      // Start calculating
      // TODO: Implement move calculation
      output << "bestmove e2e4" << endl; // Placeholder
    }
    else if ( tokens[0] == "quit" )
    {
      // Exit the program
      break;
    }
    else if ( tokens[0] == "stop" )
    {
      // Stop calculating
      // TODO: Implement stop calculation
    }
  }
}

int
main ()
{
  uci_loop ();
  return 0;
}
