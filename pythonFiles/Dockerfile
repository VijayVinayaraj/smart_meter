FROM python:3.12-alpine

ADD . /code
WORKDIR /code
RUN pip install -r requirement.txt

CMD ["python", "log_data.py"]
