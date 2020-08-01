/* A fairly straightforward program that adds together a list of 100 50-digit numbers and displays the result. */

#include <iostream>
#include <fstream>

struct Sum
{
	int val;
	Sum* next;

	Sum() : val{ 0 }, next{ nullptr } {}
	Sum(int val) : val{ val }, next{ nullptr } {}
};

typedef Sum* SumPtr;

void traverse(SumPtr& header);
// Takes in a pointer containing the address of the head of the linked list.
// Prints the value contained in each node of the inputted linked list.

void add(SumPtr& header1, SumPtr& header2);
// Takes in the pointers containing the addresses of the heads of the linked lists to be summed.
// Adds the two linked lists together.

void delList(SumPtr& header);
// Takes in the pointer containing the address of the head of the linked list to be deleted.
// Traverses the function, deleting the nodes as it progresses.

void carry(SumPtr& header, int _carry);
// Takes in the adress of the head of the linked list that the answer is being stored in, as well as the carry over from the addition.
// Recursive function that distributes a carry throughout all the nodes.

void add(SumPtr& header1, SumPtr& header2);
// Takes in the address of the heads of the linked lists containing the numbers to be added.
// Stores the sum of the two numbers in the linked list associated with header1.

void printAnswer(SumPtr& header);
// Takes in the address of the head of the linked list containing the answer.
// Displays the answer (Both the answer to the summation, and the first ten digits of the summation)

int main() {
	int i = 0;
	SumPtr header1 = nullptr;
	SumPtr header2 = nullptr;
	SumPtr temp1 = nullptr;
	SumPtr temp2 = nullptr;
	char ch;
	std::fstream fin("nums.txt", std::fstream::in);
	if (fin.is_open())
	{
		// Reads in each character into ch, which is then converted into an integer and inputted into a new node.
		// The nodes build backwards, inserting each new digit before the previous digit, resulting in a mirrored number (i.e. 12345 becomes 54321)
		while (i < 50 && fin >> std::noskipws >> ch)
		{
			if (i == 0)
			{
				header1 = new Sum(ch - '0');
				temp1 = header1;
			}
			else
			{
				header1 = new Sum(ch - '0');
				header1->next = temp1;
				temp1 = header1;
			}
			i++;
		}
		traverse(header1);				// Displays the first integer.
		for (int k = 0; k < 100; k++)
		{
			i = 0;						// Again, reads each character into ch, converts the character into an integer and stores the integer in a new node.
			while (i < 50 && fin >> std::noskipws >> ch)
			{
				if (ch >= '0' && ch <= '9')
				{
					if (i == 0)
					{
						header2 = new Sum(ch - '0');
						temp2 = header2;
					}
					else
					{
						header2 = new Sum(ch - '0');
						header2->next = temp2;
						temp2 = header2;
					}
					i++;					// incrementation within the if statement to ensure 50 digits are grabbed, and other characters ignored.
				}
			}
			traverse(header2);				// Displays the 99 other integers to be summed
			add(header1, header2);			// Adds the two linked lists together.
			delList(header2);				// Deletes the second linked list
		}

		printAnswer(header1);
	}										// If nums.txt is unable to be opened, an error message is displayed
	else
		std::cout << "nums.txt could not be opened.";
}

void traverse(SumPtr& header)
{
	SumPtr temp = header;
	while (temp != nullptr)
	{
		std::cout << temp->val;
		temp = temp->next;
	}
	std::cout << std::endl;
}

void carry(SumPtr& header, int _carry) {
	SumPtr& temp = header;

	// If no carry is produced, then the previous carry and the value contained in the node may simply be added together.
	if ((temp->val + _carry) / 10 == 0)
		temp->val = temp->val + _carry;

	// If a carry is produced:
	else {
		int tp;
		tp = (temp->val + _carry) % 10;					// tp stores the value to be stored in the node
		_carry = (temp->val + _carry) / 10;				// the carry is calculated
		temp->val = tp;									// tp is then stored into the value field within the node
		if (temp->next == nullptr)						// If the value of temp->next is nullptr, a new node must be created before taking the carry any further
			temp->next = new Sum(0);
		carry(temp->next, _carry);						// The carry function is called recursively.
	}
	// The function will repeat until the carry == 0
}

void add(SumPtr& header1, SumPtr& header2)
{
	SumPtr h1 = header1;
	SumPtr h2 = header2;

	int _carry = 0;

	while (h2 != nullptr) {								// So long as h2 does not equal nullptr, there are more digits to add.
		if (h1->next == nullptr)						// If h1->next == nullptr, a new node must be created before h1 progresses to the next node.
			h1->next = new Sum(0);
		_carry = (h1->val + h2->val) / 10;				// Carry should never be greater than 9.
		if (_carry > 0)									// If there is a carry, the carry() function is called
			carry(h1->next, _carry);
		h1->val = (h1->val + h2->val) % 10;				// The one's digit of the sum of the two numbers is stored in the value of the node.
		h1 = h1->next;									// Both linked lists advance by one digit and repeat the process until the second list is out of digits.
		h2 = h2->next;
	}

}

void delList(SumPtr& header)
{
	if (header != nullptr)
	{
		while (header != nullptr)
		{
			SumPtr temp = header->next;
			delete header;
			header = temp;
		}
	}
}

void printAnswer(SumPtr& header) {
	SumPtr temp1, temp2, temp3;			// Flips the answer around to read left to right.
	temp2 = new Sum(header->val);
	temp3 = temp2;
	temp1 = header->next;
	delete header;
	header = temp1;
	while (header != nullptr) {
		temp1 = header->next;
		temp2->next = new Sum(header->val);
		temp2 = temp2->next;
		delete header;
		header = temp1;
	}
	header = temp3;
	if (header != nullptr) {
		while (header->val == 0) {			// Gets rid of any 0s that may have wound up in front of the significant digits.
			temp1 = header->next;
			delete header;
			header = temp1;
		}
	}
	std::cout << "First 10 digits: ";
	temp1 = header;
	for (int i = 0; i < 10; i++) {
		if (temp1 != nullptr) {
			std::cout << temp1->val;
			temp1 = temp1->next;
		}
	}
	std::cout << std::endl << std::endl << "Full summation: ";
	temp1 = header;
	while (temp1 != nullptr) {
		std::cout << temp1->val;
		temp1 = temp1->next;
	}
}
