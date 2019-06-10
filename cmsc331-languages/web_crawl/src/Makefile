FLAGS =

WebCrawl.class: WebCrawl.java
	javac $(FLAGS) WebCrawl.java

# run with: make run url=https://example.com
run: WebCrawl.class
	java WebCrawl $(url)

test: WebCrawl.class
	java WebCrawl https://github.com

clean:
	rm -rf *.class
