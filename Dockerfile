FROM scratch

ADD mud .
ADD data data
ADD users users

EXPOSE 5000 5080

USER 1001

CMD [ "/mud" ]
