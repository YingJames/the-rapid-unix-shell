# for producing the final zip
ZIP_NAME = rush.zip

zip: 
	rm -f $(ZIP_NAME)
	zip -r $(ZIP_NAME) src/ -x src/.idea/\*
