# Deployment Guide

This guide covers different deployment options for the C++ HTTP Server.

## Docker Deployment

### Building the Docker Image

```bash
# Build the image
docker build -t cpp-http-server .

# Run the container
docker run -d -p 8080:8080 --name cpp-server cpp-http-server

# Check logs
docker logs cpp-server

# Stop the container
docker stop cpp-server
```

### Docker Compose (Recommended)

Create `docker-compose.yml`:

```yaml
version: '3.8'
services:
  cpp-server:
    build: .
    ports:
      - "8080:8080"
    environment:
      - LOG_LEVEL=INFO
      - SERVER_PORT=8080
      - THREAD_POOL_SIZE=4
    volumes:
      - ./logs:/app/logs
      - ./data:/app/data
    restart: unless-stopped
    healthcheck:
      test: ["CMD", "curl", "-f", "http://localhost:8080/health"]
      interval: 30s
      timeout: 10s
      retries: 3
```

Run with:
```bash
docker-compose up -d
```

## Production Deployment

### System Requirements

- Ubuntu 20.04+ or similar Linux distribution
- 2+ CPU cores
- 4GB+ RAM
- 10GB+ disk space
- Network access on port 8080

### Installation Steps

1. **Install dependencies:**
```bash
sudo apt-get update
sudo apt-get install -y libboost-all-dev nlohmann-json3-dev libssl-dev libsqlite3-dev cmake build-essential
```

2. **Create service user:**
```bash
sudo useradd -r -s /bin/false cppserver
sudo mkdir -p /opt/cpp-server
sudo chown cppserver:cppserver /opt/cpp-server
```

3. **Deploy application:**
```bash
# Copy files to production directory
sudo cp -r . /opt/cpp-server/
sudo chown -R cppserver:cppserver /opt/cpp-server

# Build application
cd /opt/cpp-server
sudo -u cppserver make clean && make
```

4. **Create systemd service:**

Create `/etc/systemd/system/cpp-server.service`:

```ini
[Unit]
Description=C++ HTTP Server
After=network.target

[Service]
Type=simple
User=cppserver
Group=cppserver
WorkingDirectory=/opt/cpp-server
ExecStart=/opt/cpp-server/server
Restart=always
RestartSec=10
Environment=LOG_LEVEL=INFO
Environment=SERVER_PORT=8080

# Security settings
NoNewPrivileges=true
ProtectSystem=strict
ProtectHome=true
ReadWritePaths=/opt/cpp-server/logs /opt/cpp-server/data

[Install]
WantedBy=multi-user.target
```

5. **Start service:**
```bash
sudo systemctl daemon-reload
sudo systemctl enable cpp-server
sudo systemctl start cpp-server
sudo systemctl status cpp-server
```

### Reverse Proxy Setup (Nginx)

Create `/etc/nginx/sites-available/cpp-server`:

```nginx
server {
    listen 80;
    server_name your-domain.com;

    location / {
        proxy_pass http://localhost:8080;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }

    location /health {
        proxy_pass http://localhost:8080/health;
        access_log off;
    }
}
```

Enable the site:
```bash
sudo ln -s /etc/nginx/sites-available/cpp-server /etc/nginx/sites-enabled/
sudo nginx -t
sudo systemctl reload nginx
```

## Environment Configuration

### Configuration Files

The server reads configuration from:
1. `config/server.conf` (default values)
2. Environment variables (override config file)

### Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `SERVER_PORT` | 8080 | Server listening port |
| `LOG_LEVEL` | INFO | Logging level (DEBUG, INFO, WARN, ERROR, FATAL) |
| `LOG_FILE` | logs/server.log | Log file path |
| `THREAD_POOL_SIZE` | 4 | Number of worker threads |
| `DB_PATH` | data/server.db | SQLite database path |
| `RSA_PRIVATE_KEY` | config/privateKey.pem | RSA private key file |
| `RSA_PUBLIC_KEY` | config/publicKey.pem | RSA public key file |
| `BCRYPT_ROUNDS` | 12 | Bcrypt hashing rounds |

### Production Environment Setup

```bash
# Create production environment file
sudo tee /opt/cpp-server/.env << EOF
SERVER_PORT=8080
LOG_LEVEL=WARN
LOG_FILE=/opt/cpp-server/logs/server.log
THREAD_POOL_SIZE=8
DB_PATH=/opt/cpp-server/data/server.db
EOF

sudo chown cppserver:cppserver /opt/cpp-server/.env
```

## Monitoring and Maintenance

### Health Checks

The server provides a health endpoint at `/health` that returns:
- Server status and uptime
- System resource usage
- Configuration details
- Dependency status

### Log Management

Set up log rotation with `/etc/logrotate.d/cpp-server`:

```
/opt/cpp-server/logs/*.log {
    daily
    rotate 30
    compress
    delaycompress
    missingok
    notifempty
    create 644 cppserver cppserver
    postrotate
        systemctl reload cpp-server
    endscript
}
```

### Performance Monitoring

Monitor key metrics:
- CPU and memory usage
- Request response times
- Error rates
- Connection counts

Use tools like:
- `htop` for system resources
- `curl http://localhost:8080/health` for application health
- `journalctl -u cpp-server -f` for real-time logs

## Scaling and Load Balancing

### Horizontal Scaling

Run multiple instances behind a load balancer:

```yaml
# docker-compose.yml for multiple instances
version: '3.8'
services:
  cpp-server-1:
    build: .
    ports:
      - "8081:8080"
    environment:
      - SERVER_PORT=8080
      
  cpp-server-2:
    build: .
    ports:
      - "8082:8080"
    environment:
      - SERVER_PORT=8080
      
  nginx:
    image: nginx:alpine
    ports:
      - "80:80"
    volumes:
      - ./nginx.conf:/etc/nginx/nginx.conf
    depends_on:
      - cpp-server-1
      - cpp-server-2
```

### Load Balancer Configuration

Nginx upstream configuration:

```nginx
upstream cpp_servers {
    server cpp-server-1:8080;
    server cpp-server-2:8080;
}

server {
    listen 80;
    
    location / {
        proxy_pass http://cpp_servers;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

## Security Considerations

### Firewall Setup

```bash
# Allow only necessary ports
sudo ufw allow 22/tcp   # SSH
sudo ufw allow 80/tcp   # HTTP
sudo ufw allow 443/tcp  # HTTPS
sudo ufw enable
```

### SSL/TLS Setup

Use Let's Encrypt with Certbot:

```bash
sudo apt-get install certbot python3-certbot-nginx
sudo certbot --nginx -d your-domain.com
```

### Security Headers

Add security headers in Nginx:

```nginx
add_header X-Frame-Options DENY;
add_header X-Content-Type-Options nosniff;
add_header X-XSS-Protection "1; mode=block";
add_header Strict-Transport-Security "max-age=31536000; includeSubDomains";
```

## Troubleshooting

### Common Issues

1. **Port already in use:**
   ```bash
   sudo lsof -i :8080
   sudo kill -9 <PID>
   ```

2. **Permission denied:**
   ```bash
   sudo chown -R cppserver:cppserver /opt/cpp-server
   sudo chmod +x /opt/cpp-server/server
   ```

3. **Missing dependencies:**
   ```bash
   ldd /opt/cpp-server/server
   sudo apt-get install --fix-missing
   ```

### Log Analysis

Check logs for issues:
```bash
# System logs
sudo journalctl -u cpp-server -n 100

# Application logs
tail -f /opt/cpp-server/logs/server.log

# Error patterns
grep -i error /opt/cpp-server/logs/server.log
```

## Backup and Recovery

### Database Backup

```bash
# Backup SQLite database
cp /opt/cpp-server/data/server.db /backup/server-$(date +%Y%m%d).db

# Automated backup script
#!/bin/bash
BACKUP_DIR="/backup"
DB_PATH="/opt/cpp-server/data/server.db"
DATE=$(date +%Y%m%d_%H%M%S)

cp "$DB_PATH" "$BACKUP_DIR/server_$DATE.db"
find "$BACKUP_DIR" -name "server_*.db" -mtime +7 -delete
```

### Configuration Backup

```bash
# Backup configuration
tar -czf /backup/cpp-server-config-$(date +%Y%m%d).tar.gz /opt/cpp-server/config
```
