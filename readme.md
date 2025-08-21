# 🚀 Authenticate

A lightweight **C++ API service** built using [Crow](https://github.com/CrowCpp/crow) (a C++ microframework for web services) and [SQLite](https://www.sqlite.org/) as the database engine.  
This project provides **user authentication**  with features like registration, login, key verification.

---

# 📂 Project Structure

```
.
├── src/
│ ├── main.cpp 
│ ├── api/
│ │ ├── authenticate/
│ │ │ ├── login.hpp 
│ │ │ └── register.hpp 
│ │ ├── users/
│ │ │ └── clients_api_verification.hpp 
│ │ └── admin/
│ │ ├── admin_api_add.hpp 
│ │ ├── admin_api_verify.hpp 
│ │ └── admin_remove_key.hpp 
│ └── ...
└── README.md

```

---

# ⚙️ Features

- ✅ User Registration (`/register`)
- ✅ User Login (`/login`)

- ✅ Client API Verification
- ✅ Uses **SQLite** for persistent storage
- ✅ Lightweight and fast (built with Crow framework)

---

# 🔧 Requirements

- **C++17 or later**
- **SQLite3**
- **Crow** (header-only, automatically included in `#include <crow.h>`)
- **OpenSSL** (for hashing and security)

---

# 🛠️ Build & Run

### 1. Clone the repo
```bash
git clone https://github.com/harshsaxena213/Authenticate.git

cd Authenticate

```
```bash
g++ main.cpp -lsqlite3 -lcrypto

./a.out (Or You Executable)
```

# 📡 Example API Usage

### Register A New User

```bash
curl -X POST http://localhost:18080/register \
-H "Content-Type: application/json" \
-d '{"username":"john_doe","password":"mypassword","email":"john@example.com"}'

```

### Login

```bash
curl -X POST http://localhost:18080/login \
-H "Content-Type: application/json" \
-d '{"username":"john_doe","password":"mypassword"}'
```

# 📖 API Endpoints Overview

| Endpoint          | Method | Description                   |
| ----------------- | ------ | ----------------------------- |
| `/register`       | POST   | Register a new user           |
| `/login`          | POST   | Authenticate user credentials |


# 🤝 Contributing

- Fork the repo

- Create a new branch (git checkout -b feature-branch)

- Commit changes (git commit -m 'Add new feature')

 - Push branch (git push origin feature-branch)

- create a Pull Request


