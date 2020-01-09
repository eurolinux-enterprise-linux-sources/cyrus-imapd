#!/bin/sh

CERT_FILE="/etc/pki/cyrus-imapd/cyrus-imapd.pem"
CERT_DIR="/etc/pki/tls/certs"

umask 077
cd "$CERT_DIR"

cat << EOF | make cyrus-imapd.pem
--
SomeState
SomeCity
SomeOrganization
SomeOrganizationalUnit
localhost.localdomain
root@localhost.localdomain
EOF

chown root.mail cyrus-imapd.pem
chmod 640 cyrus-imapd.pem
mv cyrus-imapd.pem "$CERT_FILE"
