#include "SymbolTable.h"


inline void SymbolTable::run(string filename)
{
    ifstream fname;
    fname.open(filename);
    string cmd;
	string tokens[5];
	for (int i = 0; i < 5; i++) {
		tokens[i] = "";
	}
    while (getline(fname, cmd)) {
		int num_comp = 0, num_splay = 0;
		if (split(cmd, tokens)) {
			switch (cmd[0]) {
			case 'I': {
				insert_symbol(cmd, tokens, num_comp, num_splay);
				break;
			}
			case 'A': {
				assign(cmd, tokens, num_comp, num_splay);
				break;
			}
			case 'L': {
				lookup(cmd, tokens, num_comp, num_splay);
				break;
			}
			case 'B': {
				make_new_scope();
				break;
			}
			case 'E': {
				exit_scope();
				break;
			}
			case 'P': {
				print_preorder();
				break;
			}
			}
		}
		else throw InvalidInstruction(cmd);
    }
	if (unclosed()) throw UnclosedBlock(scope_level);
}