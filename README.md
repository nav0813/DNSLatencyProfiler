DNS Latency Profiler
----------------------------------------------
Overview
-------------
The aim is to measure the latency of DNS servers of top ten domains ranked by Alexa in resolving a random query. This is achieved by periodically flooding the nameservers with random queries and measuring the latency in receiving a response. 

Design
-------------
The system measures the latency for only the Authoritative Answers (AA). It is achieved through the following steps.
1. For each of the domains, DNS query is made for its nameservers. 
2. ISP's caching DNS Resolver replies for the query. The reply is parsed to extract the A records with point to the nameserver of the domain.
3. A random query is generated for a domain. For example, 1h2kj3h.google.com for google.com.
4. The random query is sent to google.com's nameservers, which are the Authoritative Name Servers. The replies checked whether the AA bit is set and inserted into the database
5. Steps 1-4 are repeats once for every interval, that is set while starting the program.

Database Schema
----------------------
The database has two tables - TimeSeries and Aggregates. The results of every valid reply is stored in TimeSeries and statistical data for each domain i.e. average query time, standard deviation of query times, timestamp of first and last query are stored in Aggregates. The schema of both the tables are below.

Table - TimeSeries


+------------+-------------+------+-----+---------+-------+

| Field      | Type        | Null | Key | Default | Extra |

+------------+-------------+------+-----+---------+-------+

| Domain     | varchar(20) | NO   |     | NULL    |       |

| Query      | varchar(30) | NO   |     | NULL    |       |

| NameServer | varchar(16) | NO   |     | NULL    |       |

| Timestamp  | int(11)     | NO   |     | NULL    |       |

| QueryTime  | int(11)     | YES  |     | NULL    |       |

+------------+-------------+------+-----+---------+-------+




Table - Aggregates


+--------------+-------------+------+-----+---------+-------+

| Field        | Type        | Null | Key | Default | Extra |

+--------------+-------------+------+-----+---------+-------+

| AlexaRank    | int(11)     | NO   | PRI | NULL    |       |

| Domain       | varchar(20) | NO   |     | NULL    |       |

| Queries      | int(11)     | YES  |     | NULL    |       |

| AvgQueryTime | int(11)     | NO   |     | NULL    |       |

| StdDeviation | int(11)     | NO   |     | NULL    |       |

| FirstQuery   | int(11)     | NO   |     | NULL    |       |

| LastQuery    | int(11)     | NO   |     | NULL    |       |

+--------------+-------------+------+-----+---------+-------+



Pre-requisites libraries for building the system
------------------------------------------------------------
1. LDNS
2. MySQL++
3. Boost


Building the system 
------------------------------------------------------------
1. Download the contents of the repository
2. Goto Build directory and fire the command - 'make'


Running the program
-------------------------------------------------------------
1. The list of top ten domains is required to be saved in a file, with each line containing one domain and other text. The domains listed in the Challenge comes along with the repository as 'topdomains.txt'. Alternative solution is explained in the nest section.
2. Start program following the command below

    ./DNSLatencyProfiler --user <MySQL user name> --passwd <MySQL password> --database <database name> --file <file containing the top domains> --freq <frequency at which profiling repeats, in microseconds>

    ./DNSLatencyProfiler --help -> to get help in giving the command line options
    
In the ideal case, the program runs continuously until stopped. In case of errors like not being able to open the file containing the top domains or any database problems, the program halts. All the errors encountered in the execution are logged in error log files that are named as 'ErrorLog-<TimeStamp>'

Note: Different databases should be used for different sets of top domains. The password specified in the program option is not hidden.


Scraping for Alexa's top ten domains
------------------------------------------------------------
A python script named - 'getLatestTopTenAlexa.py' extracts the top ten domains from Alexa's web page and prints it to stdout. The following can be used to get the latest top ten domains, which can be used while running the program.
  
  python getLatestTopTenAlexa.pt > latestTopTen


Query results database
-------------------------------------------
As mentioned early, the query resolution results are stored in the tables - TimeSeries, and Aggregates. 'Snapshots' folder of the repository contains the snapshots of tables.

