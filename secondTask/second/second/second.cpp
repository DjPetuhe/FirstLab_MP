#include <fstream>
#include <iostream>
using namespace std;

int main()
{
	const int arrStartSize = 4;
	string txtDirectory;
	string stopWordDirectory;
	string resultDirectory;
	string temp;
	cout << "Enter directory of file with text: ";
	cin >> txtDirectory;
	cout << "\nEnter directory of file with stop words: ";
	cin >> stopWordDirectory;
	cout << "\nEnter directory of file where the result will be written: ";
	cin >> resultDirectory;
	ifstream stopWordFile(stopWordDirectory);
	string* stopWord = new string[arrStartSize];
	int currStopWordMax = arrStartSize;
	int arrStopWordSize = 0;
stopWordParse:
	stopWordFile >> temp;
	if (temp == "") goto endStopWordParse;
	temp += " ";
	stopWord[arrStopWordSize] = temp;
	temp = "";
	arrStopWordSize++;
	if (arrStopWordSize == currStopWordMax)
	{
		currStopWordMax *= 2;
		string* newArr = new string[currStopWordMax];
		int i = 0;
	fillNewArrStopWord:
		newArr[i] = stopWord[i];
		i++;
		if (i < arrStopWordSize) goto fillNewArrStopWord;
		delete[] stopWord;
		stopWord = newArr;
	}
	goto stopWordParse;
endStopWordParse:
	stopWordFile.close();
	string* words = new string[arrStartSize];
	string* wordsPages = new string[arrStartSize];
	int* wordsAmount = new int[arrStartSize];
	bool* usedThisPage = new bool[arrStartSize];
	int currWordMax = arrStartSize;
	int arrWordSize = 0;
	int line = 0;
	int pages = 0;
	int charsInWord;
	int tempPage;
	string withSymbols;
	string strInt;
	ifstream txtFile(txtDirectory);
txtParse:
	if (txtFile.peek() == '\n')
	{
		line++;
		txtFile.get();
		goto txtParse;
	}
	if (pages != line / 45)
	{
		pages++;
		int i = 0;
	boolLoop:
		if (i < arrWordSize)
		{
			usedThisPage[i] = false;
			i++;
			goto boolLoop;
		}
	}
	temp = "";
	txtFile >> temp;
	if (temp == "") goto endTxtParse;
	else
	{
		temp += " ";
		int i = 0;
	removeCapLetters:
		if (temp[i] == ' ') goto endCapLetters;
		else if (temp[i] >= 65 && temp[i] <= 90) temp[i] += 32;
		i++;
		goto removeCapLetters;
	endCapLetters:
		i = 0;
		withSymbols = "";
		charsInWord = 0;
	removeSymbols:
		if (temp[i] == ' ') goto endRemoveSymbols;
		if ((temp[i] >= 97 && temp[i] <= 122) || (charsInWord > 0 && (temp[i + 1] >= 97 && temp[i + 1] <= 122) && (temp[i] == 39 || temp[i] == 96 || temp[i] == 45)))
		{
			charsInWord++;
			withSymbols += temp[i];
		}
		i++;
		goto removeSymbols;
	endRemoveSymbols:
		if (withSymbols == "")
		{
			goto txtParse;
		}
		temp = withSymbols + " ";
		i = 0;
	checkStopWords:
		if (i < arrStopWordSize)
		{
			if (temp == stopWord[i]) goto txtParse;
			i++;
			goto checkStopWords;
		}
		i = 0;
	checkExist:
		if (i < arrWordSize)
		{
			if (temp == words[i])
			{
				wordsAmount[i]++;
				if (!usedThisPage[i])
				{
					strInt = "";
					tempPage = pages + 1;
				fromIntToStr:
					if (tempPage != 0)
					{
						strInt = char(tempPage % 10 + 48) + strInt;
						tempPage /= 10;
						goto fromIntToStr;
					}
					wordsPages[i] += ", " + strInt;
				}
				usedThisPage[i] = true;
				goto txtParse;
			}
			i++;
			goto checkExist;
		}
		words[arrWordSize] = temp;
		wordsAmount[arrWordSize] = 1;
		usedThisPage[arrWordSize] = true;
		strInt = "";
		tempPage = (line / 45) + 1;
	fromIntToStr_sec:
		if (tempPage != 0)
		{
			strInt = char(tempPage % 10 + 48) + strInt;
			tempPage /= 10;
			goto fromIntToStr_sec;
		}
		wordsPages[i] += strInt;
		temp = "";
		arrWordSize++;
	}
	if (arrWordSize == currWordMax)
	{
		currWordMax *= 2;
		string* newArr = new string[currWordMax];
		int* newArrAmount = new int[currWordMax];
		string* newArrPages = new string[currWordMax];
		bool* newUsedPages = new bool[currWordMax];
		int i = 0;
	fillNewArrWord:
		newArr[i] = words[i];
		newArrAmount[i] = wordsAmount[i];
		newArrPages[i] = wordsPages[i];
		newUsedPages[i] = usedThisPage[i];
		i++;
		if (i < arrWordSize) goto fillNewArrWord;
		delete[] words;
		delete[] wordsAmount;
		delete[] wordsPages;
		delete[] usedThisPage;
		words = newArr;
		wordsAmount = newArrAmount;
		wordsPages = newArrPages;
		usedThisPage = newUsedPages;
	}
	goto txtParse;
endTxtParse:
	txtFile.close();
	delete[] stopWord;
	int i = 0;
sort:
	if (i < arrWordSize)
	{
		string bestFound = "~";
		int j = i;
		int bestFoundJ;
	check:
		if (j < arrWordSize)
		{
			int k = 0;
		checkChars:
			if (bestFound[k] > words[j][k])
			{
				bestFoundJ = j;
				bestFound = words[j];
			}
			else if (bestFound[k] == words[j][k])
			{
				k++;
				goto checkChars;
			}
			j++;
			goto check;
		}
		temp = words[bestFoundJ];
		int tempAmount = wordsAmount[bestFoundJ];
		string tempPagesStr = wordsPages[bestFoundJ];
		words[bestFoundJ] = words[i];
		wordsAmount[bestFoundJ] = wordsAmount[i];
		wordsPages[bestFoundJ] = wordsPages[i];
		words[i] = temp;
		wordsAmount[i] = tempAmount;
		wordsPages[i] = tempPagesStr;
		i++;
		goto sort;
	}
	i = 0;
	ofstream resultFile(resultDirectory);
writeResult:
	if (i < arrWordSize)
	{
		if (wordsAmount[i] <= 100)
		{
			resultFile << words[i] << "- " << wordsPages[i] << '\n';
		}
		i++;
		goto writeResult;
	}
	resultFile.close();
	delete[] words;
	delete[] wordsAmount;
	delete[] usedThisPage;
	delete[] wordsPages;
	return 0;
}