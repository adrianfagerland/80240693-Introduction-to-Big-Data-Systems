# Homework 11

I am using the original file structure, so the code in `simple_app_python/SimpleApp.py` is all that is needed to run the script according to the instructions in the homework description.

My code extends the basic script by creating a `history` variable that stores all of the word counts, and using the function `foreachRDD()` on  `word_counts: DStream`. This function runs the custom function `get_top_100_words`, which updates the `history` variable with the current streamed file, and outputs the top 100 words in descending order to the terminal and to file.

As the homework description didn't ask for anything else, that concludes my report.
