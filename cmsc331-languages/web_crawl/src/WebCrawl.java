import java.net.*;
import java.io.*;
import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;


/*
 * Daniel Engbert CMSC 331 Fall 2016
 * This is a simple and limited web crawler.
 *
 * Takes in a command line argument of a url to visit
 * Finds the length of the HTML file corresponding to that URL
 * Finds other links on that page and adds them to a list of URLs to visit
 *
 * stops when it runs out of URLs
 * or 1,000,000 chars have been read
 * or 30 sites have been visited
 *
 * run with either of the following commands:
 * java WebCrawl https://example.com
 * make run url=https://example.com
 */
public class WebCrawl {
    static ArrayList<String> urls; // ArrayList of URLs to visit
    static ArrayList<String> visited; // ArrayList of URLs already visited
    static int totalLength = 0;
    static int limit = 1000000; // max number of character to read in


    // once there's 15 (or some number) of sites on the list, don't add anything else to it
    public static void main(String[] args) {
        urls = new ArrayList<String>();
        visited = new ArrayList<String>();

        if (args.length == 0) {
            System.out.println("Command line argument required.");
            return;
        }
        urls.add(args[0]); // add url from the command line to urls to visit

        // read from urls while limiting conditions are met
        while (urls.size() != 0 && totalLength < limit && visited.size() < 30) {
            String page = urls.get(0);
            // remove extra slash at end of url (for comparison purposes later)
            if (page.endsWith("/")) {
                page = page.substring(0, page.length()-1);
            }

            if (notVisited(page)) { // when the url hasn't already been visited
                totalLength += getLength(page); // get length of HTML file
                visited.add(page); // store visited url
            }
            urls.remove(0); // remove the url from list of urls to visit
        }

        if (totalLength >= limit)
            System.out.println("\nstopped because: total length (" + totalLength + ") exceeds limit of " + limit + " chars.");

        if (urls.size() == 0)
            System.out.println("\nstopped because: ran out of urls to visit.");

        if (visited.size() >= 30)
            System.out.println("\nstopped because: 30 urls were visited");
    }


    /*
     * returns the length (in characters) of the HTML file at the specified web page
     * also checks for <a> tag and if it exists add it to urls
     * returns 0 if the url is invalid
     */
    private static int getLength(String page) {
        String html = "";
        int length = 0;

        try {
            URL url = new URL(page);
            BufferedReader reader = new BufferedReader(new InputStreamReader(url.openStream()));

            String line;
            while ((line = reader.readLine()) != null) {
                html += line; // append the line just read to the string of html
            }

            html = html.toLowerCase(); // make html all lowercase
            length = html.length(); // get length of html (number of chars)

            System.out.println("\n" + length + " chars\tfrom " + page);

            // look for urls in anchor tags in the html
            Pattern pattern = Pattern.compile("<a\\s(?=href)href=\"(http[^\"]+)");
            Matcher matcher = pattern.matcher(html);

            while (matcher.find()) {
                urls.add(matcher.group(1)); // append the found url to the list
            }

        } catch (Exception e) {
            // url is invalid (even if the url redirects from http to https)
            System.out.println("\ninvalid url:\t" + page);
            //e.printStackTrace();
            return 0;
        }

        return length;
    }

    /*
     * returns true if the url has not been visited yet
     * the slash characters at end of some urls were already removed to make it
     * easier to compare them
     */
    private static boolean notVisited(String url) {
        for (int i=0; i<visited.size(); i++) {
            if (visited.get(i).equals(url)) { // must use equals() to compare
                return false;
            }
        }
        return true;
    }
}
