#include "curl_test.h"
#include <curl/curl.h>
#include <iostream>

int progress_func(void* ptr, double TotalToDownload, double NowDownloaded,
	double TotalToUpload, double NowUploaded)
{
	// ensure that the file to be downloaded is not empty
	// because that would cause a division by zero error later on
	if (TotalToDownload <= 0.0) {
		return 0;
	}

	// how wide you want the progress meter to be
	int totaldotz = 40;
	double fractiondownloaded = NowDownloaded / TotalToDownload;
	// part of the progressmeter that's already "full"
	int dotz = round(fractiondownloaded * totaldotz);

	// create the "meter"
	int ii = 0;
	printf("%3.0f%%\n", fractiondownloaded * 100);

	fflush(stdout);
	// if you don't return 0, the transfer will be aborted - see the documentation
	return 0;
}


size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	//size_t written = fwrite(ptr, size, nmemb, stream);
	return size * nmemb;
}

CURLM* curlm;

void curl_init() {
	curl_global_init(CURL_GLOBAL_WIN32 | CURL_GLOBAL_SSL);
	curlm = curl_multi_init();
}

void update() {
	int running_handles;
	curl_multi_perform(curlm, &running_handles);
}

void curl_test() {
	CURL* curl;
	//FILE* fp;
	CURLcode res;
	char* url = "https://file-examples.com/wp-content/uploads/2018/04/file_example_AVI_1920_2_3MG.avi";
	char outfilename[FILENAME_MAX] = "C:\\downloaded.avi";
	curl = curl_easy_init();
	if (curl) {
		//fp = fopen(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		// Internal CURL progressmeter must be disabled if we provide our own callback
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
		// Install the callback function
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
		//std::cout << "start download\n";
		//res = curl_easy_perform(curl);
		//std::cout << "end download\n";
		/* always cleanup */
		//curl_easy_cleanup(curl);
		//fclose(fp);

		curl_multi_add_handle(curlm, curl);
	}
}
