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
	int amount;
	cout << "Enter directory of file with text: ";
	cin >> txtDirectory;
	cout << "\nEnter directory of file with stop words: ";
	cin >> stopWordDirectory;
	cout << "\nEnter directory of file where the result will be written: ";
	cin >> resultDirectory;
	cout << "\nEnter amount of most common words that you want to see: ";
	cin >> amount;
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
	ifstream txtFile(txtDirectory);
	string* words = new string[arrStartSize];
	int* wordsAmount = new int[arrStartSize];
	int currWordMax = arrStartSize;
	int arrWordSize = 0;
	int charsInWord;
	string withSymbols;
txtParse:
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
		if (withSymbols == "") goto txtParse;
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
				goto txtParse;
			}
			i++;
			goto checkExist;
		}
		words[arrWordSize] = temp;
		wordsAmount[arrWordSize] = 1;
		temp = "";
		arrWordSize++;
	}
	if (arrWordSize == currWordMax)
	{
		currWordMax *= 2;
		string* newArr = new string[currWordMax];
		int* newArrAmount = new int[currWordMax];
		int i = 0;
	fillNewArrWord:
		newArr[i] = words[i];
		newArrAmount[i] = wordsAmount[i];
		i++;
		if (i < arrWordSize) goto fillNewArrWord;
		delete[] words;
		delete[] wordsAmount;
		words = newArr;
		wordsAmount = newArrAmount;
	}
	goto txtParse;
endTxtParse:
	txtFile.close();
	delete[] stopWord;
	amount = amount > arrWordSize ? arrWordSize : amount;
	int i = 0;
sortStart:
	if (i < amount)
	{
		int maxFound = 0;
		int j = i;
		int maxFoundJ;
	checkAll:
		if (j < arrWordSize)
		{
			if (maxFound < wordsAmount[j])
			{
				maxFound = wordsAmount[j];
				maxFoundJ = j;
			}
			j++;
			goto checkAll;
		}
		string tempWord = words[maxFoundJ];
		int tempAmount = wordsAmount[maxFoundJ];
		words[maxFoundJ] = words[i];
		wordsAmount[maxFoundJ] = wordsAmount[i];
		words[i] = tempWord;
		wordsAmount[i] = tempAmount;
		i++;
		goto sortStart;
	}
	ofstream resultFile(resultDirectory);
	i = 0;
writeResult:
	if (i < amount)
	{
		resultFile << words[i] << " - " << wordsAmount[i] << '\n';
		i++;
		goto writeResult;
	}
	resultFile.close();
	delete[] words;
	delete[] wordsAmount;
	return 0;
}