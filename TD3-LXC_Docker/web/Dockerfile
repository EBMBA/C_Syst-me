FROM fedora:31

RUN dnf install -y -q httpd-manual php php-cli php-gd php-mbstring php-mysqlnd php-pdo php-sodium php-xml

EXPOSE 80

ENTRYPOINT systemctl start httpd.service && systemctl enable httpd.service