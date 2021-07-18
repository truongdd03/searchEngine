# searchEngine
A search engine written in c++.

## Idea
The search engine includes 3 main processes:
- **Crawling:** Fetch html files. 
- **Parsing:** Taking web content from html files.
- **Indexing:** Store data in an effecient way.

## Implementation

### Idea
The search engine includes 3 main processes: **Crawling**, **Parsing**, **Indexing**.

### Crawling
Using the idea of the BFS algorithm, links will be store in a queue. Then, each of them will be extracted, downloading the html files. 
#### Optimization: 
- Multithreading is used to crawl multiple links at the same time.

### Parsing
After receiving HTML files from the crawling process, links and web content will be parsed by finding specific tags. The program will check whether these new links have been crawled yet. If not, they will be pushed into the queue.
- **Optimization:** Multithreading. Libcurl and regex_search are also used to simplify code.

### Indexing
The web content from the parsing process will be split into single words. Then, they will be written to file based on their first character.
- **Optimization:** 

## External libraries

- **libcurl:** Download web content.
- **Oleander Stemming Library:** Reducing the amount of saving data.
