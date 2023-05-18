FROM python:3.11-alpine

RUN apk add build-base
ADD . /code
WORKDIR /code 
RUN pip install --no-cache-dir -r requirement.txt
CMD ["uvicorn", "server:app", "--host", "0.0.0.0", "--port", "80"]
#ENTRYPOINT [ "sh" ]
