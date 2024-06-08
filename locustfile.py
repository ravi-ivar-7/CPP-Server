from locust import HttpUser, TaskSet, task, between

class MyTaskSet(TaskSet):

    @task(1)
    def upload_file(self):
        with open('./controllers/common.cpp', 'rb') as f:
            self.client.post("/api/upload", files={'file': f})

    @task(1)
    def download_file(self):
        self.client.get("/api/download")

class WebsiteUser(HttpUser):
    tasks = [MyTaskSet]
    wait_time = between(1, 2)



# RUN LOCUSR AS : locust -f locustfile.py --host=http://localhost:7000

# now got to brwoser to load test (http://localhost:8089)