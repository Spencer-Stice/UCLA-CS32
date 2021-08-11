
//Class declarations
#include <string>
using namespace std;
#include <iostream>
class Investment
{
public:
    Investment(string name, int price) 
    { m_name = name; m_price = price;};
    string name() const { return m_name; };
    virtual bool fungible() const { return false; };
    virtual string description() const  = 0;
    int purchasePrice() const { return m_price; };
    virtual ~Investment() {};
private:
    string m_name;
    int m_price;
};

class Stock : public Investment
{
public:
    Stock(string name, int cost, string ticker) : Investment(name, cost) { m_ticker = ticker; };
    string description() const { return ("stock trading as " + m_ticker); };
    virtual bool fungible() const { return true; };
    virtual ~Stock() {cout << "Destroying " + this->name() + ", a stock holding" + "\n"; };
private:
    string m_ticker;
};

class Painting : public Investment
{
public:
    Painting(string name, int cost) : Investment(name, cost) {};
    string description() const { return "painting"; };
    virtual ~Painting() { cout << "Destroying " + this->name() + ", a painting" + "\n"; };
};

class House : public Investment
{
public:
    House(string address, int cost) : Investment(address, cost) {};
    string description() const { return "house"; };
    virtual ~House() { cout << "Destroying the house " + this->name() + "\n"; };
};
void display(const Investment* inv)
{
    cout << inv->name();
    if (inv->fungible())
        cout << " (fungible)";
    cout << ": " << inv->description() << " bought for $" << inv->purchasePrice() << endl;
}

int main()
{
    Investment* portfolio[4];
    portfolio[0] = new Painting("Salvator Mundi", 450300000);
    // Stock holdings have a name, value, and ticker symbol
    portfolio[1] = new Stock("Lyft", 50000, "LYFT");
    portfolio[2] = new Stock("GameStop", 10000, "GME");
    portfolio[3] = new House("4 Privet Drive", 660000);

    for (int k = 0; k < 4; k++)
        display(portfolio[k]);

    // Clean up the investments before exiting
    cout << "Cleaning up" << endl;
    for (int k = 0; k < 4; k++)
        delete portfolio[k];
}
