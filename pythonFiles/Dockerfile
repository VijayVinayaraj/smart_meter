FROM python:3.11-alpine

RUN apk add build-base
ADD . /code
WORKDIR /codeRUN 
RUN pip install paho-mqtt==1.6
CMD ["sh", "startup.sh"]
#ENTRYPOINT [ "sh" ]
