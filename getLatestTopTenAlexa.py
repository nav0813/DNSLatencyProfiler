#! /usr/bin/python
from urllib import urlopen
import re

topDomainsPage = urlopen('http://www.alexa.com/topsites').read()
topDomainPattern = re.compile('<h2>\n<a href.*>(.*)</a>\n</h2>')
topDomains = re.findall(topDomainPattern, topDomainsPage)
topten = topDomains[:10]

for domain in topten:
	print domain

