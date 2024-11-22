# GTK-Finance-Manager-Application
This is a simple GTK-based Finance Manager application

Set a budget and track their expenses.

Add income or expense transactions with descriptions.

View a detailed report of all transactions.

Receive a warning if expenses exceed the set budget.


Features

1. Set Budget: Set an initial budget and track remaining funds.


2. Add Transactions: Add income or expense transactions with type, description, and amount.


3. View Reports: View a detailed report of all transactions in a TreeView.


4. Budget Warning: Displays a warning dialog if expenses exceed the budget.



Requirements

GTK 3.0+

Glade


Installation

1. Clone the repository:

git clone https://github.com/your-username/gtk-finance-manager.git
cd gtk-finance-manager


2. Install required dependencies:

sudo apt-get install libgtk-3-dev


3. Compile the program:

gcc -o finance_manager finance_manager.c pkg-config --cflags --libs gtk+-3.0


4. Run the application:

./finance_manager



Usage

1. Set Budget: Enter the budget in the input field and click "Set Budget."


2. Add Transactions: Click "Transaction" to open the transaction panel, fill in the details, and click "Add Transaction."


3. View Report: Click "View Report" to see a detailed report of transactions.



Files

finance_manager.c: The main C file containing the logic for the application.

finance_manager.glade: The Glade XML file for the application's GUI design.


Contributing

Contributions are welcome! Feel free to fork the repository, make changes, and submit a pull request.

License

This project is licensed under the MIT License. See the LICENSE file for more details.


---

Replace your-username in the git clone command with your GitHub username. Add any specific details about the project or screenshots if desired to make the README more engaging.
