from locust import HttpUser, TaskSet, task, between

class FileTaskTest(TaskSet):

    @task(1)
    def upload_file(self):
        with open('./controllers/common.cpp', 'rb') as f:
            self.client.post("/admin/upload", files={'file': f})

    @task(1)
    def download_file(self):
        self.client.get("/admin/download")

class SimpleTaskSet(TaskSet):
    @task(1)
    def test_async(self):
        self.client.get("/admin/testasync")

class SingleFunctionTester(TaskSet):
    @task(1)
    def test_single_function(self):
        self.client.get("/admin/home")





class WebsiteUser(HttpUser):
    tasks = [SingleFunctionTester]
    wait_time = between(1, 2)



# RUN LOCUSR AS : locust -f locustfile.py --host=http://localhost:7000 --web-port 7010

# now got to brwoser to load test (http://localhost:7010)