# 💳 Payment Gateway System (C++)

A modular and scalable **Payment Processing System** built in C++ that simulates real-world payment workflows using multiple design patterns.

---

## 🚀 Features

- Supports multiple payment gateways:
  - Paytm
  - Razorpay
- Retry mechanism for failed transactions
- Clean layered architecture (Controller → Service → Gateway)
- Extensible design (easy to add new gateways)
- Simulated payment validation and processing

---

## 🧠 Design Patterns Used

### 1. Singleton Pattern
- Used in:
  - PaymentService
  - PaymentController
  - GatewayFactory
- Ensures only one instance exists globally

### 2. Factory Pattern
- GatewayFactory creates:
  - PaytmGateway
  - RazorGateway
- Decouples object creation from usage

### 3. Template Method Pattern
- Defined in PaymentGateway
- Standard flow:
  validate → initiate → confirm
- Each gateway implements its own logic

### 4. Proxy Pattern
- PaymentGatewayProxy adds:
  - Retry mechanism
  - Logging
- Wraps actual gateway without modifying it

---

## 🏗️ System Architecture

    Client (main)
       ↓
    PaymentController (Singleton)
       ↓
    PaymentService (Singleton)
       ↓
    GatewayFactory (Singleton)
       ↓
    PaymentGatewayProxy
       ↓
    Concrete Gateway (Paytm / Razorpay)
       ↓
    Banking System

---

## 📂 Project Structure

    .
    ├── main.cpp
    ├── README.md

---

## ⚙️ How to Run

### Step 1: Compile

g++ main.cpp -o payment_system

### Step 2: Run

./payment_system

---

## 🧪 Sample Output

    🚀 Initiating Payment | Gateway: Paytm
    👤 Sender   : Aditya
    👤 Receiver : Shubham

    → Validating payment details...
    → Initiating transaction...
    → Confirming payment...

    📊 Final Status: SUCCESS ✅

---

## 🔥 Key Highlights

- Implemented 4+ design patterns in one system
- Designed scalable and extensible architecture
- Added retry logic similar to real payment systems
- Demonstrates strong OOP and Low-Level Design skills

---

## ⭐ If you like this project

Give it a star ⭐ on GitHub!
