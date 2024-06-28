from fastapi import FastAPI
import psutil
import platform
from datetime import datetime, timedelta

app = FastAPI()

@app.get("/sys-server-info")
def get_system_info():
    boot_time_timestamp = psutil.boot_time()
    bt = datetime.fromtimestamp(boot_time_timestamp)
    now = datetime.now()
    uptime = now - bt

    uname = platform.uname()
    cpu_info = f"{psutil.cpu_count(logical=False)} cores ({psutil.cpu_count()} threads)"
    mem_info = f"{psutil.virtual_memory().total // (1024 ** 3)} GB"

    return {
        "System": uname.system,
        "Node Name": uname.node,
        "Release": uname.release,
        "Version": uname.version,
        "Machine": uname.machine,
        "Processor": uname.processor,
        "Uptime": {
            "Days": uptime.days,
            "Hours": uptime.seconds // 3600,
            "Minutes": (uptime.seconds // 60) % 60,
            "Seconds": uptime.seconds % 60,
        },
        "CPU": cpu_info,
        "Memory": mem_info,
    }


# uvicorn fastapi_comparision:app --reload
