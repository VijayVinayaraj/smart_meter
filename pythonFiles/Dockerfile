FROM python:3.11-alpine

RUN apk add build-base
ADD . /code
WORKDIR /code
RUN pip install pandas
RUN pip install matplotlib
RUN pip install paho-mqtt 
CMD ["sh", "startup.sh"]
#ENTRYPOINT [ "sh" ]