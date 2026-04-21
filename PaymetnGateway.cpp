#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <memory>

using namespace std;

// ---------------------------------
// Data structure to hold payment information
// ---------------------------------

struct PaymentRequest
{
    string sender;
    string receiver;
    string currency;
    double amount;

    PaymentRequest(string s, string r, string c, double a) : sender(s), receiver(r), currency(c), amount(a) {}
};

// ---------------------------------
// Banking system interface
// ---------------------------------

class BankingSystem
{
public:
    virtual bool processPayment(double amount) = 0;
    virtual ~BankingSystem() {}
};

class PaytmBankingSystem : public BankingSystem
{
public:
    PaytmBankingSystem()
    {
        cout << "Paytm Banking System initialized." << endl;
    }
    bool processPayment(double amount) override
    {
        // Simulate payment processing with a random success/failure
        srand(time(0));
        return rand() % 3 == 0; // 33% chance of success
    }
};

class RazorpayBankingSystem : public BankingSystem
{
public:
    RazorpayBankingSystem()
    {
        cout << "Razorpay Banking System initialized." << endl;
    }
    bool processPayment(double amount) override
    {
        // Simulate payment processing with a random success/failure
        srand(time(0));
        return (rand() % 100) < 90; // 90% chance of success
    }
};

// --------------------------------------------------------------------
// Abstract base class for Payment Gateway (Template Method Pattern)
// --------------------------------------------------------------------

class PaymentGateway
{
protected:
    BankingSystem *bankingSystem;

public:
    PaymentGateway()
    {
        bankingSystem = nullptr;
    }
    virtual ~PaymentGateway() { delete bankingSystem; }

    // Template method
    virtual bool processPayment(PaymentRequest *request)
    {
        if (!validatePayment(request))
        {
            cout << "Payment gateway: Payment validation failed." << endl;
            return false;
        }
        if (!initiatePayment(request))
        {
            cout << "Payment gateway: Payment initiation failed." << endl;
            return false;
        }
        if (!confirmPayment(request))
        {
            cout << "Payment gateway: Payment confirmation failed." << endl;
            return false;
        }
        return true;
    }

    // Steps to be implemented by subclasses
    virtual bool validatePayment(PaymentRequest *request) = 0;
    virtual bool initiatePayment(PaymentRequest *request) = 0;
    virtual bool confirmPayment(PaymentRequest *request) = 0;
};

// ------------------------------------------------
// Concrete implementation of Paytm Payment Gateway
// ------------------------------------------------
class PaytmGateway : public PaymentGateway
{
public:
    PaytmGateway()
    {
        bankingSystem = new PaytmBankingSystem();
    }

    bool validatePayment(PaymentRequest *request) override
    {
        cout << "   → Validating payment details..." << endl;

        if (request->amount <= 0)
        {
            cout << "   ❌ Validation failed: Invalid amount" << endl;
            return false;
        }

        if (request->currency != "INR")
        {
            cout << "   ❌ Validation failed: Unsupported currency ("
                 << request->currency << ")" << endl;
            return false;
        }

        cout << "   ✅ Validation successful" << endl;
        return true;
    }

    bool initiatePayment(PaymentRequest *request) override
    {
        cout << "   → Initiating transaction..." << endl;
        bool result = bankingSystem->processPayment(request->amount);

        if (!result)
            cout << "   ❌ Transaction failed at bank level" << endl;
        else
            cout << "   ✅ Transaction initiated successfully" << endl;

        return result;
    }

    bool confirmPayment(PaymentRequest *request) override
    {
        cout << "   → Confirming payment..." << endl;
        cout << "   ✅ Payment confirmed for " << request->sender << endl;
        return true;
    }
};

// ------------------------------------------------
// Concrete implementation of Razorpay Payment Gateway
// ------------------------------------------------
class RazorGateway : public PaymentGateway
{
public:
    RazorGateway()
    {
        bankingSystem = new RazorpayBankingSystem();
    }

    bool validatePayment(PaymentRequest *request) override
    {
        cout << "   → Validating payment details..." << endl;

        if (request->amount <= 0)
        {
            cout << "   ❌ Validation failed: Invalid amount" << endl;
            return false;
        }

        cout << "   ✅ Validation successful" << endl;
        return true;
    }

    bool initiatePayment(PaymentRequest *request) override
    {
        cout << "   → Initiating transaction..." << endl;

        bool result = bankingSystem->processPayment(request->amount);

        if (!result)
            cout << "   ❌ Transaction failed at bank level" << endl;
        else
            cout << "   ✅ Transaction initiated successfully" << endl;

        return result;
    }

    bool confirmPayment(PaymentRequest *request) override
    {
        cout << "   → Confirming payment..." << endl;
        cout << "   ✅ Payment confirmed for " << request->sender << endl;
        return true;
    }
};

// ------------------------------------------------------------------
// Proxy for Payment Gateway
// ------------------------------------------------------------------
class PaymentGatewayProxy : public PaymentGateway
{
private:
    PaymentGateway *realGateway;
    int retries;

public:
    PaymentGatewayProxy(PaymentGateway *gateway, int maxRetries)
    {
        realGateway = gateway;
        retries = maxRetries;
    }
    ~PaymentGatewayProxy()
    {
        delete realGateway;
    }
    bool processPayment(PaymentRequest *request) override
    {
        bool result = false;
        for (int attempt = 0; attempt < retries; ++attempt)
        {
            cout << "🔁 Attempt " << (attempt + 1) << "/" << retries << endl;

            if (attempt > 0)
                cout << "   🔄 Retrying transaction..." << endl;

            result = realGateway->processPayment(request);

            if (result)
            {
                cout << "   ✅ Transaction successful" << endl;
                break;
            }
            else
            {
                cout << "   ❌ Attempt failed" << endl;
            }
        }

        if (!result)
        {
            cout << "⛔ All retry attempts exhausted" << endl;
        }

        return result;
    }

    bool validatePayment(PaymentRequest *request) override
    {
        return realGateway->validatePayment(request);
    }

    bool initiatePayment(PaymentRequest *request) override
    {
        return realGateway->initiatePayment(request);
    }

    bool confirmPayment(PaymentRequest *request) override
    {
        return realGateway->confirmPayment(request);
    }
};

// ------------------------------------------------------------------
// Gateway Factory to create payment gateways
// ------------------------------------------------------------------

enum class GatewayType
{
    PAYTM = 0,
    RAZORPAY = 1
};

class GatewayFactory
{
private:
    static GatewayFactory *instance;

    // Private constructor
    GatewayFactory() {}

    // Prevent copy
    GatewayFactory(const GatewayFactory &) = delete;
    GatewayFactory &operator=(const GatewayFactory &) = delete;

public:
    static GatewayFactory *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new GatewayFactory();
        }
        return instance;
    }

    PaymentGateway *createGateway(GatewayType type)
    {
        switch (type)
        {
        case GatewayType::PAYTM:
        {
            PaymentGateway *paytm = new PaytmGateway();
            return new PaymentGatewayProxy(paytm, 3); // retry 3 times
        }

        case GatewayType::RAZORPAY:
        {
            PaymentGateway *razor = new RazorGateway();
            return new PaymentGatewayProxy(razor, 2); // retry 2 times
        }

        default:
            cout << "GatewayFactory: Invalid Gateway Type!" << endl;
            return nullptr;
        }
    }
};

// Static member initialization
GatewayFactory *GatewayFactory::instance = nullptr;

// Unified API service (Singleton)
class PaymentService
{
private:
    static PaymentService *instance;
    PaymentGateway *gateway;
    PaymentService() : gateway(nullptr) {}
    ~PaymentService()
    {
        delete gateway;
    }

    // private constructor and delete copy/assignemnt to enforce singleton
    PaymentService(const PaymentService &) = delete;
    PaymentService &operator=(const PaymentService &) = delete;

public:
    static PaymentService *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new PaymentService();
        }
        return instance;
    }

    void setGateway(GatewayType type)
    {
        if (gateway)
        {
            delete gateway;
        }
        gateway = GatewayFactory::getInstance()->createGateway(type);
    }

    bool processPayment(PaymentRequest *request)
    {
        if (!gateway)
        {
            cout << "Payment Service: No payment gateway configured." << endl;
            return false;
        }
        return gateway->processPayment(request);
    }
};

PaymentService *PaymentService::instance = nullptr;

//----------------------------------------------------------------
// Controller class for all client requests (Singleton)
//----------------------------------------------------------------

class PaymentController
{
private:
    static PaymentController instance;
    PaymentController() {}
    ~PaymentController() {}

    // private constructor and delete copy/assignemnt to enforce singleton
    PaymentController(const PaymentController &) = delete;
    PaymentController &operator=(const PaymentController &) = delete;

public:
    static PaymentController &getInstance()
    {
        return instance;
    }

    bool handlePayment(PaymentRequest *request, GatewayType type)
    {
        string gatewayName = (type == GatewayType::PAYTM) ? "Paytm" : "Razorpay";

        cout << "\n🚀 Initiating Payment | Gateway: " << gatewayName << endl;
        cout << "──────────────────────────────────────" << endl;

        cout << "👤 Sender   : " << request->sender << endl;
        cout << "👤 Receiver : " << request->receiver << endl;

        cout << "\n🔌 Bootstrapping " << gatewayName << " Banking System..." << endl;

        PaymentService::getInstance()->setGateway(type);

        cout << "✅ System Ready\n"
             << endl;

        bool result = PaymentService::getInstance()->processPayment(request);

        cout << "\n📊 Final Status: "
             << (result ? "SUCCESS ✅" : "FAILED ❌") << endl;

        cout << "──────────────────────────────────────\n"
             << endl;

        return result;
    }
};

PaymentController PaymentController::instance;

//---------------------------------------------
// Main: Client code now goes through controller
//---------------------------------------------

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    auto req1 = make_unique<PaymentRequest>("Aditya", "Shubham", "INR", 1000.0);
    // As PaytmController is a singleton, we can directly call getInstance() to access it and handle the payment request
    bool res1 = PaymentController::getInstance().handlePayment(req1.get(), GatewayType::PAYTM);
    cout << "Results: " << (res1 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------\n\n";

    auto req2 = make_unique<PaymentRequest>("Aditya", "Shubham", "USD", 500.0);
    bool res2 = PaymentController::getInstance().handlePayment(req2.get(), GatewayType::RAZORPAY);
    cout << "Results: " << (res2 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------\n\n";
}
