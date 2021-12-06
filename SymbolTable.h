#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

struct Regex {
	string regexString = "^'[a-zA-Z0-9 ]*'$";
	string regexNumber = "^[0-9]+$";
	string regexIdentifier = "^[a-z][a-zA-Z0-9_]*$";
	string regexFunctionDec = "^\\((((number|string),)*(number|string)|)\\)->(number|string|)$";
	string regexFunctionCall = "^[a-z][a-zA-Z0-9_]*\\([a-zA-Z0-9 _,']*\\)$";
	Regex() {}
	bool isString(string s)
	{
		return regex_match(s, regex(regexString));
	}
	bool isNumber(string s)
	{
		return regex_match(s, regex(regexNumber));
	}
	bool isIdentifier(string s)
	{
		return regex_match(s, regex(regexIdentifier));
	}
	bool isFunctionDec(string s)
	{
		return regex_match(s, regex(regexFunctionDec));
	}
	bool isFunctionCall(string s)
	{
		return regex_match(s, regex(regexFunctionCall));
	}
};
inline bool check_spaces_number(string str) {
	int cnt = 0;
	for (size_t i = 0; i < str.length() && str[i] != '\''; i++) {
		if (str[i] == ' ') cnt++;
	}
	switch (str[0]) {
	case 'I': return cnt == 3;
	case 'A': return cnt > 1;
	case 'L': return cnt == 1;
	case 'B': return cnt == 0;
	case 'E': return cnt == 0;
	case 'P': return cnt == 0;
	default: return false;
	}
}
inline bool split(string str, string* result) {
	Regex my_regex = Regex();
	if (!check_spaces_number(str)) return false;
	for (int i = 0; i < 5; i++) {
		result[i] = "";
	}

	if (str[0] == 'I') {
		size_t space1 = str.find_first_of(' ');
		size_t space3 = str.find_last_of(' ');
		result[0] = str.substr(0, space1);
		string temp_str = str.substr(space1 + 1, space3 - space1 - 1);
		size_t space2 = temp_str.find_first_of(' ');
		result[1] = temp_str.substr(0, space2);
		result[2] = temp_str.substr(space2 + 1);
		result[3] = str.substr(space3 + 1);
		if (result[0] != "INSERT" || !my_regex.isIdentifier(result[1]) || (result[3] != "false" && result[3] != "true")) return false;
		if (result[2] != "number" && result[2] != "string" && !my_regex.isFunctionDec(result[2])) return false;
		if (result[1] == "number" || result[1] == "string" || result[1] == "true" || result[1] == "false") return false;
	}
	else if (str[0] == 'A') {
		size_t space1 = str.find_first_of(' ');
		string temp_str = str.substr(space1 + 1);
		size_t space2 = temp_str.find_first_of(' ');
		result[0] = str.substr(0, space1);
		result[1] = temp_str.substr(0, space2);
		result[2] = temp_str.substr(space2 + 1);
		if (result[0] != "ASSIGN" || !my_regex.isIdentifier(result[1])) return false;
		if (result[1] == "number" || result[1] == "string" || result[1] == "true" || result[1] == "false") return false;
		if (result[2] == "number" || result[2] == "string" || result[2] == "true" || result[2] == "false") return false;
		if (!my_regex.isIdentifier(result[2]) && !my_regex.isFunctionCall(result[2]) && !my_regex.isNumber(result[2]) && !my_regex.isString(result[2])) return false;
	}
	else if (str[0] == 'L') {
		size_t space1 = str.find_first_of(' ');
		result[0] = str.substr(0, space1);
		result[1] = str.substr(space1 + 1);
		if (result[0] != "LOOKUP" || !my_regex.isIdentifier(result[1])) return false;
		if (result[1] == "number" || result[1] == "string" || result[1] == "true" || result[1] == "false") return false;
	}
	else if (str[0] == 'B' || str[0] == 'E' || str[0] == 'P') {
		if (str != "END" && str != "BEGIN" && str != "PRINT") return false;
		result[0] = str;
	}
	else return false;
	return true;
}
template<class T>
struct LL
{
	struct node
	{
		T data;
		node* next;
		node(T data, node* next = NULL) {
			this->data = data;
			this->next = next;
		}
	};
	node* head;
	node* tail;
	int count;
	LL() {
		head = NULL;
		tail = NULL;
		count = 0;
	}

	void push_front(node* added) {
		if (!head) {
			head = added;
			tail = added;
			count++;
			return;
		}
		added->next = head;
		head = added;
		count++;
	}
	void push_front(T data) {
		node* added = new node(data);
		push_front(added);
	}
	void pop_front()
	{
		if (!head) return;
		if (count == 1) {
			delete head;
			head = NULL;
			tail = NULL;
			count = 0;
			return;
		}
		node* temp = head;
		head = head->next;
		temp->next = NULL;
		delete temp;
		count--;
		return;
	}
	void push_back(node* added) {
		if (!head) {
			head = added;
			tail = added;
			count++;
			return;
		}
		tail->next = added;
		tail = added;
		count++;
	}
	void push_back(T data) {
		node* added = new node(data);
		push_back(added);
	}
	void copy(LL<T>& src) {
		node* i = src.head;
		while (i) {
			this->push_back(i->data);
			i = i->next;
		}
	}
	T operator[](int i) {
		if (i == 0) return head->data;
		if (i == count - 1) return tail->data;
		int j = 0;
		node* ite = head;
		while (j < i) {
			j++;
			ite = ite->next;
		}
		return ite->data;
	}
	bool contains(string name) {
		node* i = this->head;
		while (i) {
			if (i->data.name == name) return true;
			i = i->next;
		}
		return false;
	}
	void copy_to_arr(T* arr) {
		int i = 0;
		node* ite = head;
		if (count == 0) return;
		for (; i < count; i++) {
			arr[i] = ite->data;
			ite = ite->next;
		}
	}
	void destroy() {
		if (count == 0) return;
		node* prev = head;
		node* cur = head->next;
		while (prev && cur) {
			delete prev;
			prev = cur;
			cur = cur->next;
		}
		delete prev;
		prev = NULL;
		count = 0;
	}
};

inline void split(const string& str, LL<string>& params) {
	if (str[1] == ')') {
		int pos = str.find_last_of('>');
		params.push_back(str.substr(pos + 1));
		return;
	}
	int i = 0;
	while (str[i] != ')') {
		params.push_back(str.substr(i + 1, 6));
		i += 7;
	}
	int pos = str.find_last_of('>');
	params.push_back(str.substr(pos + 1));
}
inline void split(const string& str, LL<string>& params, string& functionName) {
	int opening_bracket_pos = str.find_first_of('(');
	functionName = str.substr(0, opening_bracket_pos);
	int j = opening_bracket_pos + 1;
	int i = j;
	if (str[j] == ')') return;
	for (; str[i] != ')'; i++) {
		if (str[i] == ',') {
			params.push_back(str.substr(j, i - j));
			j = i + 1;
		}
	}
	params.push_back(str.substr(j, i - j));
}

struct Symbol {
	string type, name;
	int scope_level;
	LL<string> parameters = LL<string>();
	Symbol(string _type = "", string _name = "", int level = 0) {
		type = _type;
		name = _name;
		scope_level = level;
		parameters = LL<string>();
	}
	bool operator < (Symbol& other) {
		if (this->scope_level < other.scope_level) return true;
		else if (this->scope_level > other.scope_level) return false;
		if (this->name.compare(other.name) < 0) return true;
		return false;
	}
	bool operator == (Symbol& other) {
		return this->scope_level == other.scope_level && this->name == other.name;
	}
	bool operator > (Symbol& other) {
		return !(*this < other) && !(*this == other);
	}
	void operator = (Symbol& other) {
		this->name = other.name;
		this->type = other.type;
		this->scope_level = other.scope_level;
		this->parameters.copy(other.parameters);
	}
};

template<class T>
struct SplayTree {
	struct node {
		T data;
		node* parent, * left, * right;
		node(T data) {
			parent = NULL;
			left = NULL;
			right = NULL;
			this->data = data;
		}
	};
	node* root;

	SplayTree() {
		root = NULL;
	}
	void print_preorder(node* r) {
		if (!r) return;
		if (r != root) cout << ' ' << r->data.name << "//" << r->data.scope_level;
		else cout << r->data.name << "//" << r->data.scope_level;
		print_preorder(r->left);
		print_preorder(r->right);
	}
	bool search(T key, int& num_comp, int& num_splay, T& found) {
		node* ite = root, * prev = NULL;
		while (ite) {
			num_comp++;
			if (key < ite->data) {
				prev = ite;
				ite = ite->left;
			}
			else if (key > ite->data) {
				prev = ite;
				ite = ite->right;
			}
			else break;
		}
		if (ite) {
			splay(ite, num_splay);
			found = ite->data;
			return true;
		}
		else {
			splay(prev, num_splay);
			return false;
		}
	}
	bool search(T key, T& found, bool normal_splay) {
		node* ite = root, * prev = NULL;
		while (ite) {
			if (key < ite->data) {
				prev = ite;
				ite = ite->left;
			}
			else if (key > ite->data) {
				prev = ite;
				ite = ite->right;
			}
			else break;
		}
		if (ite) {
			if (normal_splay) splay(ite);
			found = ite->data;
			return true;
		}
		else {
			if (normal_splay) splay(prev);
			return false;
		}
	}
	void splay(node* x, int& num_splay) {
		if (!x) return;
		if (x->parent) num_splay++;
		while (x->parent) {
			if (x->parent->parent == NULL) {
				if (x->parent->left == x) {
					right_rotate(x->parent);
				}
				else {
					left_rotate(x->parent);
				}
			}
			else if (x->parent->left == x && x->parent == x->parent->parent->left) {
				right_rotate(x->parent->parent);
				right_rotate(x->parent);
			}
			else if (x->parent->right == x && x->parent == x->parent->parent->right) {
				left_rotate(x->parent->parent);
				left_rotate(x->parent);
			}
			else if (x->parent->left == x && x->parent == x->parent->parent->right) {
				right_rotate(x->parent);
				left_rotate(x->parent);
			}
			else if (x->parent->right == x && x->parent == x->parent->parent->left) {
				left_rotate(x->parent);
				right_rotate(x->parent);
			}
		}
	}
	void splay(node* x) {
		if (!x) return;
		while (x->parent) {
			if (x->parent->parent == NULL) {
				if (x->parent->left == x) {
					right_rotate(x->parent);
				}
				else {
					left_rotate(x->parent);
				}
			}
			else if (x->parent->left == x && x->parent == x->parent->parent->left) {
				right_rotate(x->parent->parent);
				right_rotate(x->parent);
			}
			else if (x->parent->right == x && x->parent == x->parent->parent->right) {
				left_rotate(x->parent->parent);
				left_rotate(x->parent);
			}
			else if (x->parent->left == x && x->parent == x->parent->parent->right) {
				right_rotate(x->parent);
				left_rotate(x->parent);
			}
			else if (x->parent->right == x && x->parent == x->parent->parent->left) {
				left_rotate(x->parent);
				right_rotate(x->parent);
			}
		}
	}
	void right_rotate(node* x) {
		node* y = x->left;
		x->left = y->right;
		if (y->right) y->right->parent = x;
		y->parent = x->parent;
		if (!x->parent) root = y;
		else if (x == x->parent->right) x->parent->right = y;
		else x->parent->left = y;
		y->right = x;
		x->parent = y;
	}
	void left_rotate(node* x) {
		node* y = x->right;
		x->right = y->left;
		if (y->left) y->left->parent = x;
		y->parent = x->parent;
		if (!x->parent) root = y;
		else if (x == x->parent->left) x->parent->left = y;
		else x->parent->right = y;
		y->left = x;
		x->parent = y;
	}
	void insert(node* add, int& num_comp, int& num_splay) {
		node* cur = root, * prev = NULL;
		while (cur)
		{
			prev = cur;
			if (add->data < cur->data) cur = cur->left;
			else cur = cur->right;
			num_comp++;
		}
		add->parent = prev;
		if (!prev) root = add;
		else if (add->data < prev->data) prev->left = add;
		else prev->right = add;
		splay(add, num_splay);
	}
	void insert(T data, int& num_comp, int& num_splay) {
		node* add = new node(data);
		insert(add, num_comp, num_splay);
	}
	node* next_root(node* i) {
		while (i->right) i = i->right;
		return i;
	}
	node* merge_subtree(node* left_tree, node* right_tree) {
		if (!left_tree) return right_tree;
		if (!right_tree) return left_tree;
		node* x = next_root(left_tree);
		splay(x);
		x->right = right_tree;
		right_tree->parent = x;
		return x;
	}
	void remove(T key) {
		node* ite = root, * del = NULL, * prev = NULL;
		if (!ite) return;
		while (ite) {
			if (ite->data == key) {
				del = ite;
				break;
			}
			if (ite->data < key) {
				prev = ite;
				ite = ite->right;
			}
			else {
				prev = ite;
				ite = ite->left;
			}
		}
		if (!del) {
			splay(prev);
			return;
		}
		splay(del);

		node* right_tree = NULL, * left_tree = NULL;
		if (del->right) {
			right_tree = del->right;
			right_tree->parent = NULL;
		}
		else right_tree = NULL;
		left_tree = del;
		left_tree->right = NULL;
		del = NULL;
		if (left_tree->left) {
			left_tree->left->parent = NULL;
		}
		root = merge_subtree(left_tree->left, right_tree);
		delete left_tree;
		left_tree = NULL;
	}
	void destroy(node* r) {
		if (!r) return;
		destroy(r->left);
		destroy(r->right);
		r->data.parameters.destroy();
		delete r;
	}
};


class SymbolTable
{
public:
	SplayTree<Symbol> tree;
	LL<LL<Symbol>> info;
	int scope_level;
	Regex myRegex = Regex();
	SymbolTable() {
		tree = SplayTree<Symbol>();
		LL<Symbol> global = LL<Symbol>();
		info = LL<LL<Symbol>>();
		info.push_front(global);
		scope_level = 0;
	}
	bool is_in_table(string name, int& found_in_scope) {
		for (int i = 0; i < info.count; i++) {
			if (info[i].contains(name)) {
				found_in_scope = info.count - 1 - i;
				return true;
			}
		}
		return false;
	}
	bool search_all(string name, string type, int scope, int& num_comp, int& num_splay, Symbol& found, bool findAll) {
		if (findAll) {
			while (scope >= 0) {
				Symbol temp = Symbol(type, name, scope);
				if (tree.search(temp, num_comp, num_splay, found)) {
					temp.parameters.destroy();
					return true;
				}
				temp.parameters.destroy();
				num_comp = 0;
				num_splay = 0;
				scope--;
			}
		}
		else {
			Symbol temp = Symbol(type, name, scope);
			if (tree.search(temp, num_comp, num_splay, found)) {
				return true;
				temp.parameters.destroy();
			}
			temp.parameters.destroy();
			return false;
		}
		return false;
	}
	bool search_all(string name, string type, int scope, Symbol& found, bool findAll, bool normal_splay) {
		if (findAll) {
			while (scope >= 0) {
				Symbol temp = Symbol(type, name, scope);
				if (tree.search(temp, found, normal_splay)) {
					temp.parameters.destroy();
					return true;
				}
				temp.parameters.destroy();
				scope--;
			}
		}
		else {
			Symbol temp = Symbol(type, name, scope);
			if (tree.search(temp, found, normal_splay)) {
				temp.parameters.destroy();
				return true;
			}
			temp.parameters.destroy();
			return false;
		}
		return false;
	}
	void insert_symbol(string& cmd, string* result, int& num_comp, int& num_splay) {
		if (result[2] == "number" || result[2] == "string") {
			int temp_static = 0;
			if (result[3] == "false") temp_static = scope_level;
			Symbol new_symbol = Symbol(result[2], result[1], temp_static);
			Symbol found = Symbol("", "", 0);
			if (tree.search(new_symbol, found, false)) {
				found.parameters.destroy();
				new_symbol.parameters.destroy();							
				throw Redeclared(cmd);
			}
			tree.insert(new_symbol, num_comp, num_splay);
			if (result[3] == "false") info.head->data.push_back(new_symbol);
			else info.tail->data.push_back(new_symbol);
			found.parameters.destroy();
			new_symbol.parameters.destroy();
			cout << num_comp << " " << num_splay << endl;
		}
		else {
			if (scope_level != 0 && result[3] == "false") throw InvalidDeclaration(cmd);
			Symbol new_symbol = Symbol(result[2], result[1], 0);
			Symbol found = Symbol("", "", 0);
			int found_in_scope = -1;
			if (is_in_table(result[1], found_in_scope)) {
				if (found_in_scope == 0) {
					found.parameters.destroy();
					new_symbol.parameters.destroy();
					throw Redeclared(cmd);
				}
			}
			split(result[2], new_symbol.parameters);
			tree.insert(new_symbol, num_comp, num_splay);
			info.tail->data.push_back(new_symbol);
			new_symbol.parameters.destroy();
			found.parameters.destroy();
			cout << num_comp << " " << num_splay << endl;
		}
	}
	void assign(string& cmd, string* result, int& num_comp, int& num_splay) {
		Symbol found = Symbol("", "", 0);
		if (myRegex.isNumber(result[2])) {			
			int found_in_scope = -1;
			if (!is_in_table(result[1], found_in_scope)) {
				found.parameters.destroy();
				throw Undeclared(cmd);
			}
			else {
				search_all(result[1], "", found_in_scope, num_comp, num_splay, found, false);
				if (found.type != "number") {
					found.parameters.destroy();
					throw TypeMismatch(cmd);
				}
				cout << num_comp << ' ' << num_splay << endl;
				found.parameters.destroy();
				return;
			}
		}
		else if (myRegex.isString(result[2])) {
			int found_in_scope = -1;
			if (!is_in_table(result[1], found_in_scope)) {
				found.parameters.destroy();
				throw Undeclared(cmd);
			}
			else {
				search_all(result[1], "", found_in_scope, num_comp, num_splay, found, false);
				if (found.type != "string") {
					found.parameters.destroy();
					throw TypeMismatch(cmd);
				}
				cout << num_comp << ' ' << num_splay << endl;
				found.parameters.destroy();
				return;
			}
		}
		else if (result[2][result[2].length() - 1] == ')') {
			LL<string> value_params = LL<string>();
			string functionName = "";				
			split(result[2], value_params, functionName);
			int found_in_scope = -1;
			if (!is_in_table(functionName, found_in_scope)) {
				found.parameters.destroy();
				value_params.destroy();
				throw Undeclared(cmd);
			}
			else {
				search_all(functionName, "", found_in_scope, num_comp, num_splay, found, false);
				if (found.scope_level != 0) {
					found.parameters.destroy();
					value_params.destroy();
					throw TypeMismatch(cmd);
				}
				if (found.type == "string" || found.type == "number") {
					found.parameters.destroy();
					value_params.destroy();
					throw TypeMismatch(cmd);
				}

				string* value_params_arr = new string[value_params.count + 1];
				value_params.copy_to_arr(value_params_arr);
				string* type_params_arr = new string[found.parameters.count + 1];
				found.parameters.copy_to_arr(type_params_arr);
				int i = 0, the_smaller_size = value_params.count < found.parameters.count ? value_params.count : found.parameters.count;
				for (; i < the_smaller_size; i++) {
					if (myRegex.isString(value_params_arr[i])) {
						if (type_params_arr[i] != "string") {
							delete[] value_params_arr;
							delete[] type_params_arr;
							found.parameters.destroy();
							value_params.destroy();
							throw TypeMismatch(cmd);
						}
					}
					else if (myRegex.isNumber(value_params_arr[i])) {
						if (type_params_arr[i] != "number") {
							delete[] value_params_arr;
							delete[] type_params_arr;
							found.parameters.destroy();
							value_params.destroy();
							throw TypeMismatch(cmd);
						}
					}
					else if (myRegex.isIdentifier(value_params_arr[i])) {
						int found_in_scope = -1;
						if (!is_in_table(value_params_arr[i], found_in_scope)) {
							delete[] value_params_arr;
							delete[] type_params_arr;
							found.parameters.destroy();
							value_params.destroy();
							throw Undeclared(cmd);
						}
						else {
							search_all(value_params_arr[i], "", found_in_scope, num_comp, num_splay, found, false);
							if (found.type != type_params_arr[i]) {
								delete[] value_params_arr;
								delete[] type_params_arr;
								found.parameters.destroy();
								value_params.destroy();
								throw TypeMismatch(cmd);
							}
						}
					}
					else {
						delete[] value_params_arr;
						delete[] type_params_arr;
						found.parameters.destroy();
						value_params.destroy();
						throw InvalidInstruction(cmd);
					}
				}
				if (value_params.count != found.parameters.count - 1) {
					delete[] value_params_arr;
					delete[] type_params_arr;
					found.parameters.destroy();
					value_params.destroy();
					throw TypeMismatch(cmd);
				}
				string function_return_type = found.parameters.tail->data;
				Symbol found3 = Symbol("", "", 0);
				if (!is_in_table(result[1], found_in_scope)) {
					found.parameters.destroy();
					found3.parameters.destroy();
					value_params.destroy();
					delete[] value_params_arr;
					delete[] type_params_arr;
					throw Undeclared(cmd);
				}
				else search_all(result[1], "", found_in_scope, num_comp, num_splay, found3, false);
				if (found3.type != function_return_type) {
					value_params.destroy();
					found.parameters.destroy();
					found3.parameters.destroy();
					delete[] value_params_arr;
					delete[] type_params_arr;
					throw TypeMismatch(cmd);
				}
				cout << num_comp << ' ' << num_splay << endl;
				value_params.destroy();
				found.parameters.destroy();
				found3.parameters.destroy();
				delete[] value_params_arr;
				delete[] type_params_arr;
			}
		}
		else {
			if (result[1] == result[2]) {
				int found_in_scope = -1;
				if (!is_in_table(result[1], found_in_scope)) {
					found.parameters.destroy();
					throw Undeclared(cmd);
				}
				else {
					search_all(result[1], "", found_in_scope, num_comp, num_splay, found, true);
					if (found.type != "number" && found.type != "string") {
						found.parameters.destroy();
						throw TypeMismatch(cmd);
					}
					num_comp++;
					cout << num_comp << ' ' << num_splay << endl;
					found.parameters.destroy();
					return;
				}
			}
			int found_in_scope = -1;
			if (!is_in_table(result[2], found_in_scope)) {
				found.parameters.destroy();
				throw Undeclared(cmd);
			}
			else {
				search_all(result[2], "", found_in_scope, num_comp, num_splay, found, false);
				string return_type = found.type;
				if (!is_in_table(result[1], found_in_scope)) {
					found.parameters.destroy();
					throw Undeclared(cmd);
				}
				else search_all(result[1], "", found_in_scope, num_comp, num_splay, found, false);
				if (found.type != return_type) {
					found.parameters.destroy();
					throw TypeMismatch(cmd);
				}
				cout << num_comp << ' ' << num_splay << endl;
				found.parameters.destroy();
			}
		}
	}
	void make_new_scope() {
		scope_level++;
		LL<Symbol> new_scope = LL<Symbol>();
		info.push_front(new_scope);
	}
	void exit_scope() {
		if (scope_level == 0) throw UnknownBlock();
		else {
			while (info.head->data.count) {
				Symbol del = info.head->data.head->data;
				tree.remove(del);
				info.head->data.pop_front();
			}
			info.pop_front();
			scope_level--;
		}
	}
	void lookup(string& cmd, string* result, int& num_comp, int& num_splay) {
		Symbol found = Symbol("", "", 0);
		int found_in_scope = -1;
		if (!is_in_table(result[1], found_in_scope)) {
			found.parameters.destroy();
			throw Undeclared(cmd);
		}
		else {
			search_all(result[1], "", found_in_scope, num_comp, num_splay, found, false);
			found.parameters.destroy();
			cout << found_in_scope << endl;
		}
	}
	void print_preorder() {
		tree.print_preorder(tree.root);
		if (tree.root) cout << endl;
	}
	bool unclosed() {
		return scope_level != 0;
	}
	void run(string filename);
	~SymbolTable() {
		while (info.head) {
			while (info.head->data.head) {
				info.head->data.head->data.parameters.destroy();
				info.head->data.pop_front();
			}
			info.pop_front();
		}
		tree.destroy(tree.root);
	}
};
#endif