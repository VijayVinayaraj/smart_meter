FROM python:3.11-alpine

RUN apk add build-base
ADD . /code
WORKDIR /code
RUN pip install pandas==2.0
RUN pip install matplotlib==3.7
RUN pip install paho-mqtt==1.6
CMD ["sh", "startup.sh"]
#ENTRYPOINT [ "sh" ]