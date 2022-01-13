#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>

using namespace std;

class Inventory {
	int productID, id, quantity, inventoryQty;
	long double price, restockPrice;
	int choice;
	string inventoryMaster[16][8];
	string outofstockMaster[16][8];
public:
	void menu();
private:
	void adminMenu();
	void customerMenu();
	void restockItems();
	void buyItems();
	bool tryAgain();
	void loadInv();
	void loadoutofStock();
	void checkoutofStock();
	void showInv();
	void showSales();
	void showPurchases();
	void showoutofStocks();
	void updatePurchases();
	void updateSales();
	void updateInv();
};


void Inventory::menu() {
	system("cls");
	loadInv();
	 RECT Window;

    MoveWindow(GetConsoleWindow(), 0,0 , GetSystemMetrics(SM_CXSCREEN) ,  GetSystemMetrics(SM_CYSCREEN), TRUE);// Gets the size of the monitor display in order to perfectly center the console
    GetWindowRect(GetConsoleWindow(), &Window);
	cout << "ABC Trading Company" << endl;
	cout << "1. Admin Menu" << endl;
	cout << "2. Customer Menu" << endl;
	cout << "0. Exit" << endl;
	cout << "Choose a number: ";
	cin >> choice;
	choice = toupper(choice);

	if (choice == 1) {
		adminMenu();
	}
	else if (choice == 2) {
		customerMenu();
	}
	else if (choice == 0) {
		exit(0);
	}
	else {
		cout << "INVALID INPUT!!!" << endl;
		cin.clear();
		cin.ignore();
		system("pause");
		menu();
	}
}

void Inventory::adminMenu() {
	do{
	system("cls");
	cout << "1. Show Inventory" << endl;
	cout << "2. Show Sales" << endl;
	cout << "3. Show Purchases" << endl;
	cout << "4. Show Out of Stocks" << endl;
	cout << "5. Exit" << endl;
	cout << "Choose a number: ";
	cin >> choice;
	choice = toupper(choice);

	if (choice == 1) {
		showInv();
	}
	else if (choice == 2) {
		showSales();
	}
	else if (choice == 3) {
		showPurchases();
	}
	else if (choice == 4) {
		checkoutofStock();
	}
	else if (choice == 5) {
	}
	else {
		cout << "INVALID INPUT!!!" << endl;
		cin.clear();
		cin.ignore();
	}
	}while(choice != 5);
	
}

void Inventory::customerMenu() {
	system("cls");

	cout << "1. Check Items" << endl;
	cout << "2. Buy Items" << endl;
	cout << "Choose a number: ";
	cin >> choice;

	if (choice == 1) {
		showInv();
		cout << "Press any key to go back." << endl;
		system("pause");
		customerMenu();
	}
	else if (choice == 2) {
		buyItems();
	}
	else {
		cout << "INVALID INPUT!!!" << endl;
		cin.clear();
		cin.ignore();
		system("pause");
		customerMenu();
	}
}

void Inventory::showInv() {
	system("cls");
	cout << "Product ID	Product Desc.	    	Supplier  		Item Price  		Item Qty  	Reorder Point	Category  		UOM	\n";
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 8; j++) {
			cout << inventoryMaster[i][j] << "\t\t";
		}
		cout << endl;
	}
	cout << "\n\n Press Enter to continue";
	cin.clear();
	cin.ignore(1000,'\n');
	system("pause");
	adminMenu();
}

void Inventory::buyItems() {
	system("cls");

	showInv();

	for (int i = 1; i < 16; i++) {
		if (stoi(inventoryMaster[i][4]) > 0)
			break;
		else if (i == 15 && stoi(inventoryMaster[i][4]) <= 0) {
			cout << "\n\nAll items are out of stock." << endl;
			system("pause");
			customerMenu();
		}
	}

	cout << "\n BUY ITEMS\n\n";
	cout << "Choose product ID: " << endl;
	while (true) {
		cin >> productID;
		if (!cin || productID < 1 || productID > 15 || stoi(inventoryMaster[productID][4]) == 0) {
			cout << "INVALID INPUT!!! You must input between 1-15 or not out of stock. Try Again? " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		else break;
	}

	cout << "Choose quantity: " << endl;
	while (true) {
		cin >> quantity;
		if (!cin || quantity < 1 || quantity > stoi(inventoryMaster[productID][4])) {
			cout << "INVALID INPUT!!! Input quantity must not be below 1 or exceed the available quantity of the item. Try again? " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		else break;
	}
	inventoryQty = stoi(inventoryMaster[productID][4]);
	inventoryMaster[productID][4] = to_string(inventoryQty - quantity);
	price = stold(inventoryMaster[productID][3]) * quantity;

	showInv();
	cout << "\nPurchased: " << quantity << " " << inventoryMaster[productID][7] << " of " << inventoryMaster[productID][1] << " for " << price << " pesos. " << endl;
	updateInv();
	updateSales();
	checkoutofStock();

	cout << "Would you like to buy more items [Y/N]? ";
	cin >> choice;
	choice = toupper(choice);

	if (tryAgain() == true)
		buyItems();
	else
		menu();
}
bool Inventory::tryAgain() {
	switch (choice) {
	case 'Y':
		return true;
		break;
	case 'N':
		return false;
		break;
	default:
		cout << "INVALID INPUT!!!" << endl;
		cin.clear();
		system("pause");
		tryAgain();
	}
}
void Inventory::updateInv() {
	ofstream inv;
	
	inv.open("inventoryMasterFile.txt", ios::out);
	cout << "Product ID	Product Desc.	    	Supplier  		Item Price  		Item Qty  	Reorder Point	Category  	 	UOM";
	if (inv.is_open()) {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 8; j++) {
				inv << inventoryMaster[i][j] << "\t\t";
			}
			cout <<endl;
		}
	}inv.close();
}

void Inventory::loadInv() {
	ifstream inv;
	string line;
	inv.open("inventoryMasterFile.txt");
	getline(inv, line);
	int i = 0;
	while(inv >> inventoryMaster[i][0] >> inventoryMaster[i][1]>> inventoryMaster[i][2]>> inventoryMaster[i][3]>> inventoryMaster[i][4]>> inventoryMaster[i][5]>> inventoryMaster[i][6]>> inventoryMaster[i][7]){
		i++;
	}
}

void Inventory::updateSales() {
	ofstream sales;
	string line;
	sales.open("Sales.txt", ios::app);
	if (sales.is_open()) {
		sales << endl;
		sales << inventoryMaster[productID][1] << "," << quantity << " " << inventoryMaster[productID][7] << "," << price << " pesos";
	}sales.close();
}

void Inventory::showSales() {
	system("cls");

	ifstream sales;
	string line;
	sales.open("Sales.txt", ios::in);
	if (sales.is_open()) {
		int i = 0;
		while (getline(sales, line, ',')) {
			cout << line << "\t";
			if (i < 4)
				cout << "\t";
			i++;
		}
	}sales.close();
	cout << "\n\n\nPress any key to go back." << endl;
	system("pause");
	adminMenu();
}

void Inventory::loadoutofStock() {
	ifstream stock;
	string line;
	stock.open("outofStock.txt", ios::in);
	if (stock.is_open()) {
		int i = 0, j = 0;
		while (getline(stock, line, ',')) {
			if (j == 8) {
				i++;
				j = 0;
			}
			outofstockMaster[i][j] = line;
			j++;
		}
	}stock.close();
}

void Inventory::checkoutofStock() {
	ofstream file;
	system("cls");
	file.open("outofstock.txt");
	for(int i = 0; i <15; i++){
		if(stoi(inventoryMaster[i][4]) <= stoi(inventoryMaster[i][5])){
			for(int j  = 0; j < 8; j++){
				file << inventoryMaster[i][j] << "\t";
				cout<< inventoryMaster[i][j] << "\t";
			}
			cout << endl;
			file << endl;
		}
	}
	file.close();
	cout << "\n\n";
	cout << "1. Restock Out of Stock Item" << endl;
	cout << "2. Go back to menu." << endl;
	cout << "Choose a number: ";
	cin >> choice;
	choice = toupper(choice);

	if (choice == 1) {
		restockItems();
	}
	else if (choice == 2) {
		adminMenu();
	}
	else {
		cout << "INVALID INPUT!!!" << endl;
		cin.clear();
		system("pause");
		checkoutofStock();
	}
}

void Inventory::showoutofStocks() {
	system("cls");
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 8; j++) {
			if (i == 0 && j == 2)
				cout << "\t";
			if (i > 0 && j < 3)
				cout << "\t";
			if (i > 0 && j > 3)
				cout << "\t";
			cout << outofstockMaster[i][j] << "\t";
		}
	}
	cout << "\n\n";
	cout << "1. Restock Out of Stock Item" << endl;
	cout << "2. Go back to menu." << endl;
	cout << "Choose a number: ";
	cin >> choice;
	choice = toupper(choice);

	if (choice == 1) {
		restockItems();
	}
	else if (choice == 2) {
		adminMenu();
	}
	else {
		cout << "INVALID INPUT!!!" << endl;
		cin.clear();
		system("pause");
		showoutofStocks();
	}
}

void Inventory::restockItems() {
	for (int i = 1; i < 16; i++) {
		if (stoi(inventoryMaster[i][4]) <= stoi(inventoryMaster[i][5]))
			break;
		else if (i == 15 && stoi(inventoryMaster[i][4]) >= stoi(inventoryMaster[i][5])) {
			cout << "There are currently no Out Of Stock Items." << endl;
			system("pause");
			adminMenu();
		}
	}
	int restockQty, newQty;
	cout << "\n\nChoose ID of out of stock item: ";
	while (true) {
		cin >> id;
		id--;
		if (!cin || id < 1 || id > 15 || stoi(inventoryMaster[id][4]) > stoi(inventoryMaster[id][5])) {
			cout << "INVALID INPUT!!! Input must be between 1-15 and must be out of stock. Try again? " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		else break;
	}
	cout << "How much would you like to order: ";
	while (true) {
		cin >> restockQty;
		if (!cin || restockQty < stoi(inventoryMaster[id][5])) {
			cout << "INVALID INPUT!!! Reorder can't be below Reorder Point (ROP). Try again? " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		else break;
	}
	newQty = restockQty + stoi(inventoryMaster[id][4]);
	inventoryMaster[id][4] = to_string(newQty);
	restockPrice = stold(inventoryMaster[id][3]) * restockQty;

	updateInv();
	updatePurchases();
	checkoutofStock();

	cout << "\n\nItem ID " << id << " restocked for " << restockQty << " units at the price of " << restockPrice << " pesos." << endl;
	cout << "Would you like to restock more items [Y/N]? ";
	cin >> choice;
	choice = toupper(choice);
	if (tryAgain() == true)
		restockItems();
	else
		menu();
	showoutofStocks();
}

void Inventory::showPurchases() {
	system("cls");
	ifstream purchases;
	string line;
	purchases.open("Purchases.txt", ios::in);
	if (purchases.is_open()) {
		int i = 0;
		while (getline(purchases, line, ',')) {
			cout << line << "\t";
			if (i < 4)
				cout << "\t";
			i++;
		}
	}purchases.close();
	cout << "\n\n\nPress any key to go back." << endl;
	system("pause");
	adminMenu();
}

void Inventory::updatePurchases() {
	ofstream purchases;
	string line;
	purchases.open("Purchases.txt", ios::app);
	if (purchases.is_open()) {
		purchases << endl;
		purchases << inventoryMaster[id][1] << "," << inventoryMaster[id][4] << " " << inventoryMaster[id][7] << "," << restockPrice << " pesos" << endl;
	}purchases.close();
}

int main()
{
	Inventory inventory;
	inventory.menu();

	return 0;
}
