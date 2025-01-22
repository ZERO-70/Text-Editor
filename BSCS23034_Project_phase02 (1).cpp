#include<iostream>
#include<vector>
#include <conio.h> 
#include<fstream>
#include<string>
#include <utility> 
#include<algorithm>
#include<Windows.h>
using namespace std;

template<class T>
class doubly_linkedlist;

class Texteditor;


void moveToTopLeft() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { 0, 0 }; // Top-left corner
	SetConsoleCursorPosition(console, position);
}

class SearchEngine {
	string keyword;
	vector<pair<int, int>> occurences;
	int current;
public:
	SearchEngine() {
		keyword = "";
	}
	void setKeyword(const string& kw) {
		keyword = kw;
	}
	void keyword_index(const string& line, int y) {
		size_t index = line.find(keyword);
		while (index != string::npos) {
			occurences.push_back({ y,index });
			index = line.find(keyword, index + 1);
		}
	}
	void find_ocurrences(const vector<string>& text) {
		for (int y = 0; y < text.size(); y++) {
			keyword_index(text[y], y);
		}
		current = -1;
	}
	pair<int, int> find_next() {
		if (occurences.size() < 1) return pair<int, int>(-1, -1);
		if (current >= static_cast<int>(occurences.size() - 1)) return pair<int, int>(-1, -1);
		current++;
		return occurences[current];
	}
	pair<int, int> find_previous() {
		if (occurences.size() < 1) return pair<int, int>(-1, -1);
		if (current < 1) return pair<int, int>(-1, -1);
		current--;
		return occurences[current];
	}
	void replace_in_line(string& line, const int& x, const string& replacement, const string& keyword) {
		string newline = line.substr(0, x > 0 ? x : 0);
		newline = newline + replacement + line.substr(x + keyword.size());
		line = newline;
	}
	void replace_all_inline(string& line, const string& replacement, const string& keyword,int y) {
		for (int i = 0; i < occurences.size(); i++) {
			if (occurences[i].first == y) {
				replace_in_line(line, occurences[i].second, replacement, keyword);
				clear();
				setKeyword(keyword);
				keyword_index(line, y);
			}
		}
	}
	void replace_first_inline(string& line, const string& replacement, const string& keyword) {
		if (occurences.size() < 1) return;
		replace_in_line(line, occurences[0].second, replacement, keyword);
	}
	void clear() {
		occurences.clear();
		keyword = "";
		int current = -1;
	}
	vector<pair<int, int>>& get_occurences() {
		return this->occurences;
	}
	bool exists(pair<int, int> target) {
		for (int i = 0; i < occurences.size(); i++) {
			if (target == occurences[i])
				return true;
		}
		return false;
	}
	string get_keyword() {
		return keyword;
	}
};


template<class T>
class node {
	friend class doubly_linkedlist<T>;
	friend class Texteditor;
	T data;
	node* next;
	node* previous;
public:
	node(const T& data, node<T>* previous = nullptr, node<T>* next = nullptr) {
		this->data = data;
		this->previous = previous;
		this->next = next;
	}

};
template<class T>
class doubly_linkedlist {
	node<T>* head;
	node<T>* tail;
	int size;
	//general iterator...
	class forward_iterator {
		node<T>* iterator_node;
	public:
		forward_iterator(node<T>* the_node = nullptr) : iterator_node(the_node) {};
		forward_iterator& operator++() {
			this->iterator_node = this->iterator_node->next;
			return *this;
		}
		forward_iterator operator++(int) {
			node<T>* temp = this->iterator_node;
			++(*this);
			return temp;
		}
		bool operator!=(const forward_iterator& other) {
			return this->iterator_node != other.iterator_node;
		}
		bool operator==(const forward_iterator& other) {
			return this->iterator_node == other.iterator_node;
		}
		T& operator*() {
			return this->iterator_node->data;
		}
		node<T>* getnode() {
			return this->iterator_node;
		}
		forward_iterator& operator--() {
			this->iterator_node = this->iterator_node->previous;
			return *this;
		}
		void setnode(node<T>* node) {
			this->iterator_node = node;
		}
	};
	class backward_iterator {
		node<T>* iterator_node;
	public:
		backward_iterator(node<T>* the_node = nullptr) : iterator_node(the_node) {};
		backward_iterator& operator--() {
			this->iterator_node = this->iterator_node->previous;
			return *this;
		}
		backward_iterator operator--(int) {
			node<T>* temp = this->iterator_node;
			--(*this);
			return temp;
		}
		bool operator!=(const backward_iterator& other) {
			return this->iterator_node != other.iterator_node;
		}
		T& operator*() {
			return this->iterator_node->data;
		}
		node<T>* getnode() {
			return this->iterator_node;
		}
	};
	friend class Texteditor;

public:
	doubly_linkedlist(node<T>* h = nullptr, node<T>* t = nullptr) :head(h), tail(t),size(0) {}
	~doubly_linkedlist() {
		while (head)
			pop_front();
	}
	void add(doubly_linkedlist<T>& list) {
		auto it = list.begin();
		while (it != list.end()) {
			T d = *it;
			this->push_back(d);
			it++;
		}
	}
	void pop() {
		while (head != nullptr) {
			pop_back();
		}
	}
	void push_front(const T& d) {
		node<T>* newNode = new node<T>(d);
		if (!head) {
			this->head = this->tail = newNode;
		}
		else {
			this->head->previous = newNode;
			newNode->next = head;
			this->head = newNode;
		}
		size++;
	}
	void push_back(const T& d) {
		node<T>* newNode = new node<T>(d);
		if (!tail) {
			this->head = this->tail = newNode;
		}
		else {
			this->tail->next = newNode;
			newNode->previous = this->tail;
			this->tail = newNode;
		}
		size++;
	}
	void pushat(const T& d, int index) {
		index--;
		if (index <= 0) {
			this->push_front(d);
			return;
		}
		index--;
		node<T>* temp = this->head;
		while (temp != nullptr && index--) {
			temp = temp->next;
		}
		if (temp == this->tail->next) {
			this->push_back(d);
			return;
		}
		node<T>* n = new node<T>(d, temp, temp->next);
		if (this->tail != temp)
			temp->next->previous = n;
		temp->next = n;
		size++;
	}
	void pushat(const T& d, node<T>* element) {
		if (head == nullptr) {
			head = element;
			tail = head;
			return;
		}
		if (head == element) {
			push_front(d);
			return;
		}
		node<T>* temp = head;
		while (temp->next != nullptr && element != temp->next) {
			temp = temp->next;
		}
		node<T>* newnode = new node<T>(d, temp, temp->next);
		if (temp->next != nullptr) {
			temp->next->previous = newnode;
		}
		temp->next = newnode;
		size++;
	}
	void popat(node<T>* element) {
		if (head == nullptr) {
			return;
		}
		if (head == element) {
			return;
		}
		node<T>* temp = head;
		while (temp->next != nullptr && element != temp->next) {
			temp = temp->next;
		}
		if (temp->previous != nullptr)
			temp->previous->next = temp->next;
		// next of temp will never be nullptr, handles this case at head == element condition.
		if (head == temp) {
			head = temp->next;
		}
		if (temp->next != nullptr) {
			temp->next->previous = temp->previous;
		}
		delete temp;
		size--;
	}
	void pop_at(node<T>* element) {
		if (head == nullptr) {
			return;
		}
		if (head == element) {
			return;
		}
		node<T>* temp = head;
		while (temp->next != nullptr && element != temp) {
			temp = temp->next;
		}
		if (temp->previous != nullptr)
			temp->previous->next = temp->next;
		if (head == temp) {
			head = temp->next;
		}
		if (temp->next != nullptr)
			temp->next->previous = temp->previous;
		if (temp == tail) {
			tail = temp->previous;
		}
		delete temp;
		size--;
	}
	void pop_front() {
		node<T>* temp = this->head;
		if (!this->head) return;
		else if (this->head == this->tail) {
			this->head = this->tail = nullptr;
		}
		else {
			this->head = this->head->next;
			this->head->previous = nullptr;
		}
		delete temp;
		size--;
	}
	void pop_back() {
		node<T>* temp = this->tail;
		if (!this->tail) return;
		else if (this->head == this->tail) {
			this->head = this->tail = nullptr;
		}
		else {
			this->tail = this->tail->previous;
			this->tail->next = nullptr;
		}
		delete temp;
		size--;
	}
	forward_iterator begin() {
		return forward_iterator(head);
	}
	forward_iterator end() {
		return forward_iterator(nullptr);
	}
	backward_iterator rbegin() {
		return backward_iterator(tail);
	}
	backward_iterator rend() {
		return backward_iterator(this->head->previous);
	}
	forward_iterator get_mid() {
		if (this->head == nullptr) {
			return forward_iterator();
		}
		auto slow = this->begin();
		auto fast = this->begin();

		while (fast != this->end()) {
			++fast;
			if (!(fast != this->end())) {
				break;
			}
			++fast;
			++slow;
		}
		return slow;
	}
	void swap_helper() {
		if (this->head->next == nullptr) {
			return;
		}
		forward_iterator i = this->begin();
		swap_nodes(i);
	}
	void swap_nodes(forward_iterator i) {
		if (i.getnode() == this->tail->next || i.getnode()->next == this->tail->next) {
			return;
		}
		forward_iterator n = i;
		n++;
		if (this->tail == n.getnode()) {
			this->tail = i.getnode();
		}
		i.getnode()->next = n.getnode()->next;
		n.getnode()->previous = i.getnode()->previous;

		n.getnode()->next = i.getnode();
		i.getnode()->previous = n.getnode();
		if (this->head == i.getnode()) {
			this->head = n.getnode();
		}
		else {
			n.getnode()->previous->next = n.getnode();
		}
		if (this->tail != i.getnode()) {
			i.getnode()->next->previous = i.getnode();
		}
		i++;
		swap_nodes(i);
	}
	void print() {
		forward_iterator it = this->begin();
		for (; it != this->end(); ++it) {
			cout << *it;
		}
		cout << endl;
	}
	void print(forward_iterator cursor,int y ,SearchEngine& ing) {
		forward_iterator it = this->begin();
		bool start_hightlight = false;
		int x = 0;
		int count = 0;
		for (; it != this->end(); ++it,x++) {
			if (it == cursor) {
				if (ing.get_occurences().size() > 0 && (
					ing.exists(pair<int, int>(y, x))
					|| start_hightlight)) {
					if (!start_hightlight) {
						count = x + ing.get_keyword().size();
					}
					if (x >= count) {
						start_hightlight = false;
						cout << "\033[32;41m" << *it << "\033[0m";
					}
					else {
						start_hightlight = true;
						cout << "\033[32;41m" << *it << "\033[0m";
					}
				}
				else
					cout << "\033[32;41m" << *it << "\033[0m";
			}
			else {
				if (ing.get_occurences().size() > 0 && (
					ing.exists(pair<int, int>(y, x))
					|| start_hightlight)) {
					if (!start_hightlight) {
						count = x + ing.get_keyword().size();
					}
					if (x>= count) {
						start_hightlight = false;
						cout << *it;
					}
					else {
						start_hightlight = true;
						cout << "\033[32;45m" << *it << "\033[0m";
					}
				}
				else
					cout << *it;
			}
		}
		cout << endl;
	}
	void swap_nodes_iterative() {
		if (this->head->next == nullptr) {
			return;
		}
		forward_iterator it = this->begin();
		for (; it.getnode() != this->tail->next && it.getnode()->next != this->tail->next;) {
			forward_iterator n = it;
			n++;

			it.getnode()->next = n.getnode()->next;
			n.getnode()->previous = it.getnode()->previous;

			it.getnode()->previous = n.getnode();
			n.getnode()->next = it.getnode();

			if (this->head == it.getnode()) {
				this->head = n.getnode();
			}
			else {
				n.getnode()->previous->next = n.getnode();
			}
			if (this->tail == n.getnode()) {
				this->tail = it.getnode();
			}
			else {
				it.getnode()->next->previous = it.getnode();
			}
			it++;
		}
	}
	void insert_as_sort(const T& d) {
		node<T>* temp = this->head;
		int index = 0;
		while (temp != this->tail->next) {
			index++;
			if (d < temp->data) {
				break;
			}
			temp = temp->next;
		}
		if (temp == this->tail->next) {
			push_back(d);
			return;
		}
		this->pushat(d, index);
	}
	void rotate(int n) {
		if (n > 0) {
			backward_iterator t = this->rbegin();
			while (n-- && t.getnode() != nullptr) {
				t--;
			}

			if (t.getnode() == nullptr) {
				return;
			}

			node<T>* a = t.getnode()->next;
			t.getnode()->next = nullptr;

			this->tail->next = this->head;
			this->head->previous = this->tail;

			a->previous = nullptr;

			this->head = a;

			this->tail = t.getnode();

		}
		else if (n < 0) {

			n = n * (-1);

			forward_iterator t = this->begin();

			while (n-- && t.getnode() != nullptr) {
				t++;
			}

			if (t.getnode() == nullptr) {
				return;
			}

			node<T>* z = t.getnode()->previous;

			t.getnode()->previous = nullptr;

			this->tail->next = this->head;
			this->head->previous = this->tail;

			z->next = nullptr;

			this->head = t.getnode();

			this->tail = z;

		}
	}
	node<T>* getTail() {
		return this->tail;
	}
	int get_size() {
		return size;
	}
	void concate(doubly_linkedlist& list) {
		if (head == nullptr && tail == nullptr) {
			head = list.begin().getnode();
			tail = list.getTail();
			return;
		}
		tail->next = list.begin().getnode();
		tail->next->previous = this->tail;
		tail = list.getTail();
		this->size += list.get_size();
	}

};


class FileManager {
	string filename;
	ifstream getfile;
	ofstream tofile;
	bool have_been_modified;
public:
	FileManager(const string& filename = "") {
		this->filename = filename;
		this->have_been_modified = false;
	}
	void readFile(vector<string>& v) {
		if (filename.size() <= 0) {
			cout << "file name is not given..." << endl;
			return;
		}
		getfile.open(filename);
		if (!getfile.is_open()) {
			cout << "unable to open file for writing ...." << endl;
			return;
		}
		string line;
		while (getline(getfile, line)) {
			v.push_back(line);
		}
		getfile.close();
	}
	void writeFile(vector<string>& v) {
		if (filename.size() <= 0) {
			cout << "file name is not given..." << endl;
			return;
		}
		tofile.open(filename);
		if (!tofile.is_open()) {
			cout << "unable to open file for writing ...." << endl;
			return;
		}
		for (int i = 0; i < v.size(); i++) {
			tofile << v[i];
			tofile << "\n";
		}
		tofile.close();
	}
	void set_modified(bool check) {
		have_been_modified = check;
	}
	void set_filename(const string& fn) {
		filename = fn;
	}
	string get_filename() {
		return this->filename;
	}
	bool get_modified() {
		return this->have_been_modified;
	}
	bool fileExists(const std::string& filename) {
		ifstream file(filename);  
		return file.is_open();         
	}
	bool filePermission(const std::string& filename) {
		ifstream file(filename);
		if (!file.is_open()) {
			if (errno == EACCES) {
				return false;
			}
		}
		return true;
	}

};


class Texteditor {
	vector<doubly_linkedlist<char>*> lines;
	doubly_linkedlist<char>::forward_iterator cursor;
	vector<doubly_linkedlist<char>*>::iterator current_line;
	int upper_boundary;
	int lower_boundary;
	int mode;
	string command;
	string command_status;
	doubly_linkedlist<char> yank;
	FileManager filemanager;
	SearchEngine searchengine;
	vector<string> command_history;
	int command_index;

	void clear_memory() {
		for (auto ptr : lines)
			delete ptr;
		lines.clear();
	}
public:
	Texteditor(const string& fn = "") {
		cursor = nullptr;
		lines.push_back(new doubly_linkedlist<char>());
		lines[0]->push_back(' ');
		current_line = lines.begin();
		cursor.setnode((*current_line)->getTail());
		upper_boundary = 0;
		lower_boundary = 32;
		mode = 0;
		command = "";
		command_status = "";
		command_index = 0;
		filemanager.set_filename(fn);

		if (fn.size() > 0) {
			load_file();
		}
	}
	void insert(char c) {
		/*if (cursor.getnode() == lines[current_line].getTail()) {
			lines[current_line].push_back(c);
			cursor = lines[current_line].getTail();
		}
		else {
			lines[current_line].pushat(c, cursor.getnode());
		}
		*/

		(*current_line)->pushat(c, cursor.getnode());
		filemanager.set_modified(true);
	}
	void moveCursorLeft() {
		if (cursor.getnode()->previous != nullptr) {
			--cursor;
		}
		else {
			if ((current_line-lines.begin()) > 0) {
				--current_line;
				cursor.setnode((*current_line)->getTail());
			}
		}
	}
	int moveCursorRight() {
		if (cursor.getnode()->next != nullptr) {
			++cursor;
		}
		else {
			if (static_cast<int>(current_line - lines.begin()) < lines.size()-1) {
				++current_line;
				cursor = (*current_line)->begin();
			}
			else {
				return 0;
			}
		}
		return 1;
	}
	void moveCursorUp() {
		int index = cal_index_of_cursor();
		if ((current_line - lines.begin()) > 0) {
			--current_line;
			cursor = (*current_line)->begin();
			if (index < (*current_line)->get_size()) {
				while (index > 0) {
					cursor++;
					index--;
				}
			}
			else {
				cursor = (*current_line)->getTail();
			}

			if ((current_line - lines.begin()) < upper_boundary) {
				upper_boundary--;
				lower_boundary--;
			}
		}
	}
	void moveCursorDown() {
		int index = cal_index_of_cursor();
		if (static_cast<int>(current_line - lines.begin()) < lines.size() - 1) {
			current_line++;
			cursor = (*current_line)->begin();
			if (index < (*current_line)->get_size()) {
				while (index > 0) {
					cursor++;
					index--;
				}
			}
			else {
				cursor = (*current_line)->getTail();
			}
			if ((current_line - lines.begin()) >= lower_boundary) {
				lower_boundary++;
				upper_boundary++;
			}
		}
	}
	void createNewLine() {
		int index = static_cast<int>(current_line - lines.begin());
		if (index >= lines.size() - 1) {
			lines.push_back(new doubly_linkedlist<char>());
			current_line = lines.begin() + index + 1;
			(*current_line)->push_back(' ');
			cursor.setnode((*current_line)->getTail());
		}
		else {
			auto it = current_line;
			it++;
			lines.insert(it, new doubly_linkedlist<char>());
			current_line = lines.begin() + index + 1;
			(*current_line)->push_back(' ');
			cursor.setnode((*current_line)->getTail());
		}
		if ((current_line - lines.begin()) >= lower_boundary) {
			lower_boundary++;
			upper_boundary++;
		}
		filemanager.set_modified(true);
	}
	int cal_index_of_cursor() {
		int index = 0;
		auto t = (*current_line)->begin();
		while (t != nullptr && t != cursor) {
			t++;
			index++;
		}
		return index;
	}
	void backspace() {
		if (cursor.getnode() == (*current_line)->head && (current_line - lines.begin()) > 0) {
			if ((current_line - lines.begin()) == upper_boundary) {
				upper_boundary--;
				lower_boundary--;
			}
			doubly_linkedlist<char>* temp = *current_line;
			current_line--;
			if (cursor.getnode()->next != nullptr){
				if ((*current_line)->get_size() > 1) {
					(*current_line)->pop_back();
					cursor = (*current_line)->getTail();
					(*current_line)->concate(*temp);
				}
				else {
					int index = static_cast<int>(current_line - lines.begin());
					(*current_line)->pop_back();
					lines[index] = new doubly_linkedlist<char>();
					current_line = lines.begin() + index;
					(*current_line)->concate(*temp);
					cursor = (*current_line)->begin();
				}
				
			}
			else {
				cursor = (*current_line)->getTail();
				delete temp;
			}
			lines.erase(lines.begin() + (current_line - lines.begin())+1);
			
		}
		else {
			(*current_line)->popat(cursor.getnode());
		}
		filemanager.set_modified(true);
	}
	void del() {
		if (cursor.getnode()->next != nullptr) {
			(*current_line)->pop_at(cursor.getnode()->next);
		}
		filemanager.set_modified(true);
	}
	void del_char_at_cursor() {
		if (cursor.getnode() == (*current_line)->tail) {
			return;
		}
		if (cursor.getnode() == (*current_line)->head) {
			if (cursor.getnode()->next != nullptr) {
				node<char>* d = cursor.getnode();
				cursor++;
 				(*current_line)->pop_at(d);
			}
			else {
				backspace();
			}
		}
		else {
			node<char>* d = cursor.getnode();
			--cursor;
			(*current_line)->pop_at(d);
		}
		filemanager.set_modified(true);
	}
	void delete_current_line() {
		if ((current_line - lines.begin()) == upper_boundary && (current_line - lines.begin()) > 0) {
			upper_boundary--;
			lower_boundary--;
		}
		if (lines.size() > 1) {
			doubly_linkedlist<char>* temp = *current_line;
			int offset = static_cast<int>(current_line - lines.begin());
			lines.erase(lines.begin() + (current_line - lines.begin()));
			delete temp;
			current_line = lines.begin();
			offset--;
			while (offset>0) {
				current_line++;
				offset--;
			}
			cursor = (*current_line)->getTail();
		}
		else {
			while ((*current_line)->get_size() > 1) {
				cursor = (*current_line)->getTail();
				backspace();
			}
		}
		filemanager.set_modified(true);
	}
	void delete_nth_line(int n) {
		if (n >= lines.size() || n < 0) return;
		if (n == (current_line - lines.begin())) { delete_current_line(); return; }
		auto temp = lines[n];
		if (lines.size()>1) {
			doubly_linkedlist<char>* temp = lines[n];
			int offset = static_cast<int>(current_line - lines.begin());
			int cursor_index = cal_index_of_cursor();
			lines.erase(lines.begin() + n);
			delete temp;
			current_line = lines.begin();
			offset--;
			while (offset > 0) {
				current_line++;
				offset--;
			}
			cursor = (*current_line)->begin();
			while (cursor_index > 0) {
				cursor++;
				cursor_index--;
			}
		}
		else {
			while ((*current_line)->get_size() > 1) {
				cursor = (*current_line)->getTail();
				backspace();
			}
		}
		filemanager.set_modified(true);
	}
	void delete_to_end_of_line() {
		while (cursor.getnode()->next != nullptr) {
			del();
		}
		(*current_line)->push_back(' ');
		filemanager.set_modified(true);
	}
	void dis() {
		moveToTopLeft();
		int i = upper_boundary; 
		while (i < lower_boundary ) {
			if (i < lines.size()) {
				cout << "\033[32;40m" << i << '.'<< "\033[0m";
				lines[i]->print(cursor,i,searchengine);
			}
			else {
				int id = cal_index_of_cursor();
				while (id < 146) {
					cout << ' ';
					id++;
				}
				cout << endl;
			}
			i++;
		}
		string modified = "";
		if (filemanager.get_modified()) {
			modified += "[+]";
		}
		if (mode ==1)
			cout << "\033[32;40m" << "INSERT" << "   Total lines :" << lines.size() << "   X:"<<cal_index_of_cursor()<< "  Y:"<< (current_line - lines.begin())<<"   FileName: " << filemanager.get_filename()<< " "+modified << "   CMD_STATUS: " << command_status << "\033[0m";
		else if (mode == 0)
			cout << "\033[32;40m" << "VIEW" << "   Total lines :" << lines.size() << "   X:" << cal_index_of_cursor() << "  Y:" << (current_line - lines.begin()) << "   FileName: " << filemanager.get_filename() << " " + modified << "   CMD_STATUS: " << command_status << "\033[0m";
		else if (mode == 2) {
			cout << "\033[32;40m" << ":" << "\033[0m" << command << "\033[32;40m" << "   Total lines :" << lines.size() << "   X:" << cal_index_of_cursor() << "  Y:" << (current_line - lines.begin()) << "   FileName:" << filemanager.get_filename() << " " + modified << "   CMD_STATUS: " << command_status << "\033[0m";
		}
		else if (mode == 3)
			cout << "\033[32;40m" << "FIND" << "   Total lines :" << lines.size() << "   X:" << cal_index_of_cursor() << "  Y:" << (current_line - lines.begin()) << "   FileName: " << filemanager.get_filename() << " " + modified << "   CMD_STATUS: " << command_status << "\033[0m";

	}
	void display() {
		int i = upper_boundary;
		while (i < lower_boundary) {
			if (i < lines.size()) {
				cout << "\033[32;40m" << i << '.' << "\033[0m";
				lines[i]->print(cursor, i, searchengine);
			}
			else {
				cout << endl;
			}
			i++;

		}
		string modified = "";
		if (filemanager.get_modified()) {
			modified += "[+]";
		}
		if (mode == 1)
			cout << "\033[32;40m" << "INSERT" << "   Total lines :" << lines.size() << "   X:" << cal_index_of_cursor() << "  Y:" << (current_line - lines.begin()) << "   FileName: " << filemanager.get_filename() << " " + modified << "   CMD_STATUS: " << command_status << "\033[0m";
		else if (mode == 0)
			cout << "\033[32;40m" << "VIEW" << "   Total lines :" << lines.size() << "   X:" << cal_index_of_cursor() << "  Y:" << (current_line - lines.begin()) << "   FileName: " << filemanager.get_filename() << " " + modified << "   CMD_STATUS: " << command_status << "\033[0m";
		else if (mode == 2) {
			cout << "\033[32;40m" << ":" << "\033[0m" << command << "\033[32;40m" << "   Total lines :" << lines.size() << "   X:" << cal_index_of_cursor() << "  Y:" << (current_line - lines.begin()) << "   FileName:" << filemanager.get_filename() << " " + modified << "   CMD_STATUS: " << command_status << "\033[0m";
		}
		else if (mode == 3)
			cout << "\033[32;40m" << "FIND" << "   Total lines :" << lines.size() << "   X:" << cal_index_of_cursor() << "  Y:" << (current_line - lines.begin()) << "   FileName: " << filemanager.get_filename() << " " + modified << "   CMD_STATUS: " << command_status << "\033[0m";

	}
	void copy_line() {
		yank.add(*(*current_line));
	}
	void paste_after() {
		if (yank.get_size() <= 0) {
			return;
		}
		auto it = current_line;
		it++;
		if (it != lines.end()) {
			int index = static_cast<int>(current_line - lines.begin());
			lines.insert(it, new doubly_linkedlist<char>());
			current_line = lines.begin() + index;
			it = current_line;
			++it;
			(*it)->add(yank);
			yank.pop();
		}
		else {
			int index = static_cast<int>(current_line - lines.begin());
			lines.push_back(new doubly_linkedlist<char>());
			current_line = lines.begin() + index;
			it = current_line;
			it++;
			(*it)->add(yank);
			yank.pop();
		}

	}
	void paste_before() {
		if (yank.get_size() <= 0) {
			return;
		}
		auto it = current_line;
		int index = static_cast<int>(current_line - lines.begin());
		lines.insert(it, new doubly_linkedlist<char>());
		current_line = lines.begin() + index;
		it = current_line;
		current_line++;
		(*it)->add(yank);
		yank.pop();
	}
	void move_to_begin_of_next_word() {
		while (cursor.getnode()->data != ' ' ) {
			if (cursor.getnode()->data == ',' || cursor.getnode()->data == '.') {
				break;
			}
			if (!moveCursorRight()) {
				break;
			}
		}
		while (cursor.getnode()->data == ' ' || cursor.getnode()->data == ',' || cursor.getnode()->data == '.') {
			if (!moveCursorRight()) {
				break;
			}
		}
	}
	void move_to_begin_of_previous_word() {
		while (cursor.getnode()->data != ' ') {
			if (cursor.getnode()->data == ',' || cursor.getnode()->data == '.' || (cursor.getnode()->previous == nullptr && (current_line - lines.begin()) <= 0)) {
				break;
			}
			moveCursorLeft();
		}
		while (cursor.getnode()->data == ' ' || cursor.getnode()->data == ',' || cursor.getnode()->data == '.') {
			moveCursorLeft();
		}
		while (true) {
			if (cursor.getnode()->previous == nullptr || cursor.getnode()->previous->data == ' ' || cursor.getnode()->previous->data == ',' || cursor.getnode()->previous->data == '.') {
				break;
			}
			moveCursorLeft();
		}
	}
	void get_command(int prev_mode)
	{
		system("cls");
		display();
		char c = ' ';
		while (c != 13) {
			c = _getch();
			if (c == -32) {
				c = _getch();
				if (c == 72) {
					if (command_history.size() > 0 && command_index > 0) {
						command_index--;
						command = command_history[command_index];
					}
					
				}
				if (c == 80) {
					if (command_index < static_cast<int>(command_history.size() - 1)) {
						command_index++;
						command = command_history[command_index];
					}
					else {
						command_index = static_cast<int>(command_history.size());
						command = "";
					}
					
				}
			}
			else {
				if (c != 13 && c != 8) {
					command += c;
				}
				if (c == 8 && command.size() > 0) {
					command.pop_back();
				}
			}
			system("cls");
			display();
		}
		execute_command(prev_mode);
	}
	void load_file() {

		clear_memory();

		// constructor initialization subpart
		cursor = nullptr;
		lines.push_back(new doubly_linkedlist<char>());
		lines[0]->push_back(' ');
		current_line = lines.begin();
		cursor.setnode((*current_line)->getTail());
		upper_boundary = 0;
		lower_boundary = 32;
		mode = 0;
		//

		vector<string> data;
		filemanager.readFile(data);
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) {
				insert(data[i][j]);
			}
			createNewLine();
		}
		filemanager.set_modified(false);
	}
	void update_text(vector<string>& data) {
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) {
				insert(data[i][j]);
			}
			createNewLine();
		}
		filemanager.set_modified(true);
	}
	void join_current_with_next() {
		if (((current_line - lines.begin()) == lines.size() - 1) || lines.size() <= 1) {
			return;
		}
		auto next_line = current_line;
		next_line++;
		(*current_line)->getTail()->next = (*next_line)->head;
		(*next_line)->head->previous = (*current_line)->getTail();
		(*current_line)->tail = (*next_line)->tail;
		lines.erase(lines.begin() + (current_line - lines.begin()) + 1);
	}

	void load_text(vector<string>& v) {
		auto i = lines.begin();
		auto j = (*i)->begin();
		string line = "";
		for (; i != lines.end();) {
			j = (*i)->begin();
			for (; j != (*i)->end(); ++j) {
				line += (*j);
			}
			line.erase(line.begin() + (line.size() - 1));
			v.push_back(line);
			line = "";
			++i;
		}
	}
	string extract_current_line() {
		node<char>* p = (*current_line)->begin().getnode();
		string line = "";
		while (p != nullptr) {
			line += p->data;
			p = p->next;
		}
		return line;
	}
	void set_current_line(const string& line) {
		int indexofcursor = cal_index_of_cursor();
		doubly_linkedlist<char>* temp = *current_line;
		*current_line = new doubly_linkedlist<char>();
		(*current_line)->push_back(' ');
		cursor.setnode((*current_line)->getTail());
		for (int i = 0; i < line.size()-1; i++) {
			insert(line[i]);
		}
		cursor = (*current_line)->begin();
		while (indexofcursor>0 && cursor.getnode()->next != nullptr) {
			cursor++;
			indexofcursor--;
		}
		delete temp;
	}
	string extract_filename() {
		auto i = command.begin();
		i++;
		while (i != command.end() && *i == ' ') {
			++i;
		}
		if (i == command.end()) {
			command_status = "filename not found";
			return "";
		}
		string fn = "";
		while (i != command.end()) {
			fn += *i;
			i++;
		}
		return fn;
	}
	void save_file(string fn = "") {
		
		if (!(fn == "")) {
			this->filemanager.set_filename(fn);
		}
		vector<string> data;
		load_text(data);
		filemanager.writeFile(data);

		command_status = " Succeed";

		filemanager.set_modified(false);
	}
	void Find(string command) {
		if (command.size() < 2) {
			command_status = "No Pattern Given to Find";
			mode = 0;
			searchengine.clear();
			return;
		}
		string pattren = command.substr(1);
		searchengine.setKeyword(pattren);
		vector<string> text;
		load_text(text);
		searchengine.find_ocurrences(text);
		cursor_postion_tofind(searchengine.find_next());

	}
	void cursor_postion_tofind(const pair<int,int>& position) {
		if (position.first == -1 && position.second == -1) {
			command_status = "Keyword Not Found";
			return;
		}
		command_status = "";
		while ((current_line - lines.begin()) < position.first) {
			moveCursorDown();
		}
		while ((current_line - lines.begin()) > position.first) {
			moveCursorUp();
		}
		cursor = (*current_line)->begin();
		int x = position.second;
		while (x--) {
			cursor++;
		}
	}
	pair<string,string> extract_FindReplace() {
		auto i = command.begin();
		i++;
		i++;
		string finded = "";
		while (*i != '/') {
			finded += *i;
			i++;
			if (i == command.end()) {
				command_status = "INCOMPLETE";
				return pair<string,string>();
			}
		}
		i++;
		string replacement = "";
		while (i != command.end() && *i != '/') {
			replacement += *i;
			i++;
			
		}
		return { finded,replacement };

	}
	void Find_Replace() {
		pair<string, string> words = extract_FindReplace();
		searchengine.clear();
		if (command_status != "INCOMPLETE") {
			string line = extract_current_line();
			if (line.size() < 1) return;
			int line_index = static_cast<int>(current_line - lines.begin());
			searchengine.setKeyword(words.first);
			searchengine.keyword_index(line, line_index);
			searchengine.get_occurences().size() == 0 ? command_status = "NONE FOUND" : command_status = "";
			searchengine.replace_first_inline(line,words.second,words.first);
			searchengine.clear();
			set_current_line(line);
		}
	}
	void Find_Replace_all() {
		pair<string, string> words = extract_FindReplace();
		searchengine.clear();
		if (command_status != "INCOMPLETE") {
			string line = extract_current_line();
			if (line.size() < 1) return;
			int line_index = static_cast<int>(current_line - lines.begin());
			searchengine.setKeyword(words.first);
			searchengine.keyword_index(line, line_index);
			searchengine.get_occurences().size() == 0? command_status = "NONE FOUND": command_status = "";
			int y = static_cast<int>(current_line - lines.begin());
			searchengine.replace_all_inline(line, words.second, words.first,y);
			searchengine.clear();
			set_current_line(line);
		}
	}
	bool is_command_find_replace_all() {
		auto i = command.begin();
		i++;
		i++;
		while (i != command.end() && *i != '/') {
			i++;
		}
		if (i == command.end()) {
			command_status = "INCOMPLETE";
			return false;
		}
		i++;
		while (i != command.end() && *i != '/') {
			i++;
		}
		if (i == command.end()) {
			return false;
		}
		i++;
		if (*i == 'g') {
			return true;
		}
		else {
			command_status = "INCOMPLETE";
			return false;
		}
	}
	int extract_line_number_to_del() {
		auto i = command.begin();
		i++;
		while (i != command.end() && *i == ' ') {
			i++;
		}
		if (i == command.end()) {
			command_status = "INCOMPLETE";
			return -1;
		}
		string c = "";
		while (i != command.end()) {
			c += *i;
			i++;
		}
		return stoi(c);
	}
	int extract_number_of_lines_to_delete() {
		string number = "";
		auto i = command.begin();
		while (i != command.end() && *i != 'd') {
			number += *i;
			i++;
		}
		return stoi(number);
	}
	int extract_number_of_lines_to_yank() {
		string number = "";
		auto i = command.begin();
		while (i != command.end() && *i != 'y') {
			number += *i;
			i++;
		}
		return stoi(number);
	}
	int extract_number_of_lines_to_down() {
		string number = "";
		auto i = command.begin();
		while (i != command.end() && *i != 'j') {
			number += *i;
			i++;
		}
		return stoi(number);
	}
	void yank_this_lines(int n) {
		auto temp = current_line;
		while (temp != lines.end() && n > 0) {
			yank.add(*(*temp));
			temp++;
			n--;
		}
	}
	void execute_command(int prev_mode) {
		if (command == "dd" && prev_mode == 0) {
			command_status = "";
			delete_current_line();
			command_history.push_back(command);
			command_index = static_cast<int>(command_history.size()) ;
		}
		if (command == "D" && prev_mode == 0) {
			command_status = "";

			delete_to_end_of_line();
			command_history.push_back(command);
			command_index = static_cast<int>(command_history.size()) ;
		}
		if (command == "x" && prev_mode == 0) {
			command_status = "";

			del_char_at_cursor();
			command_history.push_back(command);
			command_index = static_cast<int>(command_history.size()) ;
		}
		if (command == "yy") {
			copy_line();
			command_history.push_back(command);
			command_index = command_history.size() ;
		}
		if (command == "p") {
			paste_after();
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command == "P") {
			paste_before();
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command == "0") {
			cursor = (*current_line)->begin();
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command == "$") {
			cursor = (*current_line)->getTail();
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command == "w") {
			move_to_begin_of_next_word();
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command == "b") {
			move_to_begin_of_previous_word();
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command[0] == 'w' && command.size() > 2) {
			string fn = extract_filename();
			if (filemanager.get_filename() == "" && filemanager.fileExists(fn)) {
				command_status = "File Already exits, overide? (Y/N)";
				filemanager.set_filename(fn);
			}
			else {
				save_file(fn);
				command_status = "FILE SAVED";
			}
			command_history.push_back(command);
			command_index = command_history.size();

		}
		if ((command == "Y" || command == "y") && command_status == "File Already exits, overide? (Y/N)") {
			save_file(filemanager.get_filename());
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if ((command == "N" || command == "n") && command_status == "File Already exits, overide? (Y/N)") {
			filemanager.set_filename("");
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if ((command == "Y"||command=="y") && command_status == "Unsaved changes,Sure to exit? (Y/N)") {
			exit(0);
		}
		if ((command == "N" || command == "n") && command_status == "Unsaved changes,Sure to exit? (Y/N)") {
			command_status = "";
			command_index = command_history.size();
		}
		if (command == "q") {
			if (filemanager.get_modified()) {
				command_status = "Unsaved changes,Sure to exit? (Y/N)";
			}
			else {
				exit(0);
			}
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command == "q!") {
			exit(0);
		}
		if (command == "wq") {
			if (filemanager.get_filename() != "") {
				save_file();
				exit(0);
			}
			command_status = "No filename found";
			command_index = command_history.size();
		}
		if (command[0] == 'e' && command.size() > 2) {
			string fn = extract_filename();
			if (!filemanager.filePermission(fn)) {
				command_status = "Permission Not allowed";
			}
			else if (!filemanager.fileExists(fn)) {
				command_status = "File Not Found";
			}
			else {
				filemanager.set_filename(fn);
				load_file();
			}
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command[0] == '/') {
			Find(command);
			command_history.push_back(command);
			mode = 3;
			command_index = command_history.size();
			command = "";
			return;
		}
		if (command[0] == 's' && is_command_find_replace_all()) {
			Find_Replace_all();
			command_history.push_back(command);
			command_index = command_history.size();
			prev_mode = 0;
			if (command_status != "NONE FOUND")
				command_status = "REPLACED";
		}
		if (command[0] == 's') {
			Find_Replace();
			command_history.push_back(command);
			command_index = command_history.size();
			prev_mode = 0;
			if (command_status != "NONE FOUND")
				command_status = "REPLACED";
		}
		if (command[0] == 'd' && command != "dd") {
			int n = extract_line_number_to_del();
			if (command_status != "INCOMPLETE") {
				delete_nth_line(n);
				command_history.push_back(command);
			}
			command_index = command_history.size();
		}
		if (command == "J") {
			join_current_with_next();
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command.size()>0 && command[command.size() - 1] == 'd' && command[command.size() - 2] == 'd' && command.size()>2 && prev_mode == 0) {
			int n;
			try {
				n = extract_number_of_lines_to_delete();
			}
			catch (const std::invalid_argument&) {
				command_status = "INVALID";
				command = "";
				mode = prev_mode;
				return;
			}
			if (n > lines.size()-1) {
				command_status = "OVERFLOW HANDELED";
			}
			for (int i = 0; i < n; i++) {
				delete_current_line();
			}
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command.size() > 0 && command[command.size() - 1] == 'y' && command[command.size() - 2] == 'y' && command.size() > 2) {
			int n;
			try {
				n = extract_number_of_lines_to_yank();
			}
			catch (const std::invalid_argument&) {
				command_status = "INVALID";
				command = "";
				mode = prev_mode;
				return;
			}
			yank_this_lines(n);
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command.size() > 0 && command[command.size() - 1] == 'j' && command.size() > 1) {
			int n;
			try {
				n = extract_number_of_lines_to_down();
			}
			catch (const std::invalid_argument&) {
				command_status = "INVALID";
				command = "";
				mode = prev_mode;
				return;
			}
			for (int i = 0; i < n; i++) {
				moveCursorDown();
			}
			command_history.push_back(command);
			command_index = command_history.size();
		}
		if (command == ">>") {
			add_indent();
			command_status = "Indented 4 spaces";
		}
		if (command == "<<") {
			remove_indent();
			if (command_status != "No indent found , Although removed spaces if any") {
				command_status = "Removed Indent";
			}
		}
		command = "";
		mode = prev_mode;
	}
	void add_indent() {
		int index = cal_index_of_cursor();
		cursor = (*current_line)->begin();
		insert(' ');
		insert(' ');
		insert(' ');
		insert(' ');
		index += 3;
		while (index > 0 && cursor != (*current_line)->getTail()) {
			cursor++;
			index--;
		}
	}
	void remove_indent() {
		int index = cal_index_of_cursor();
		cursor = (*current_line)->begin();
		int check = 4;
		while (cursor.getnode()->data == ' ' && cursor.getnode() != (*current_line)->getTail()) {
			cursor++;
			backspace();
			check--;
		}
		index -= 4;
		if (index < 0) index = 0;
		while (index > 0 && cursor != (*current_line)->getTail()) {
			cursor++;
			index--;
		}
		if (check > 0 ) {
			command_status = "No indent found , Although removed spaces if any";
		}
	}
	
	void run() {
		char c;
		_getch();
		display();
		while (true) {
			c = _getch();
			if (c == -32) {
				c = _getch();
				switch (c) {
				case 72:
					moveCursorUp();
					break;
				case 80:
					moveCursorDown();
					break;
				case 75:
					moveCursorLeft();
					break;
				case 77:
					moveCursorRight();
					break;
				}
			}
			else if (c == 13 && mode == 1){
				createNewLine();
			}
			else if (c == 8 && mode ==1) {
				backspace();
			}
			else if (c == 105 && mode != 1) {
				mode = 1;
			}
			else if (c == 27 && mode != 0) {
				mode = 0;
			}
			else if (c == 58 && mode != 2) {
				int prev_mode = mode;
				mode = 2;
				get_command(prev_mode);
			}
			else if (c == 110 && mode == 3) {
				cursor_postion_tofind(searchengine.find_next());
			}
			else if (c == 78 && mode == 3) {
				cursor_postion_tofind(searchengine.find_previous());
			}
			else if (mode ==1){
				insert(c);
			}
			system("cls");
			display();
		}
	}
};

void main() {
	Texteditor t;
	t.run();
}