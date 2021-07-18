# searchEngine
A search engine written in c++.


## Implementation

### Idea
The search engine includes 3 main processes: **Crawling**, **Parsing**, **Indexing**.

### Crawling
Using the idea of the BFS algorithm, links will be store in a queue. Then, each of them will be extracted, downloading the html files. 
##### Optimization: 
- Multithreading: used to crawl multiple links at the same time.
- Bloom filter: detect crawled links.

### Parsing
After receiving HTML files, links and web content will be parsed by finding specific tags. These links will be pushed into the queue in the crawling process.
##### Optimization: 
- Multithreading.
- Libcurl and regex_search: used to simplify code.

### Indexing
The web content from the parsing process will be split into single words. Then, they will be written to files based on their first character.
##### Optimization:
- Oleander Stemming Library: Stem words to reduce data storage.

### Search results
The ranking is based on the number of times the keywords appear on each page. Instead of starting crawling again each time the user search, this program will return results based on the indexed data, hence reduce search time.


## External libraries

- [curlpp:](http://www.curlpp.org) Download web content.
- [Oleander Stemming Library:](http://www.oleandersolutions.com/stemming/stemming.html) Reducing the amount of saving data.
