# Authentication Module

Handle user authentication using secure password hashing with bcrypt. Keep user credentials safe and provide reliable authentication services.

## What It Does

- Securely hashes passwords using the industry-standard bcrypt algorithm
- Verifies user credentials during login attempts
- Protects against common password attacks through salt and adaptive hashing

## How to Use It

### Authentication Endpoint
- **URL**: `/authenticate`
- **Method**: `GET`
- **What it does**: Authenticates users based on JSON credentials sent
- **Perfect for**: Login systems, API authentication, user verification

## Dependencies

- [Boost Libraries](https://www.boost.org/) - For robust C++ functionality
- [nlohmann::json](https://github.com/nlohmann/json) - For easy JSON handling
- [bcrypt](https://github.com/rg3/bcrypt) - For secure password hashing


