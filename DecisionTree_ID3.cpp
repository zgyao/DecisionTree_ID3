#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class MatrixCls
{
  private:
    vector < vector < string > > Matrix;
  public:
    MatrixCls(string Data_File)
    {
      Matrix.erase(Matrix.begin(),Matrix.end());
      ifstream Data(Data_File);
      string line;
      string item;
      vector < string > Row;
      while(!Data.eof())
    	{
    		getline(Data, line);
    		istringstream iss(line);
    		while(iss >> item)
    		{
    			if (item.length())
    			{
    				Row.push_back(item);
    			}
    		}

    		if(line.length())
    		{
    			Matrix.push_back(Row);
    			Row.erase(Row.begin(),Row.end());
    		}
    	}
      Data.close();
    }

    MatrixCls(vector < vector < string > > A_Matrix)
    {
      for(int i = 0; i < A_Matrix.size(); i++)
      {
        Matrix.push_back(A_Matrix[i]);
      }
    }

    MatrixCls(){ };
    ~MatrixCls(){ };

    string Element(int i,int j)
    {
      return Matrix[i][j];
    }

    int SizeX()
    {
      return Matrix[0].size();
    }

    int SizeY()
    {
      return Matrix.size();
    }

    vector < string > GetAttributes()
    {
      vector < string > Attribute;
      int j;
      for(j = 0; j < Matrix[0].size()-1; j++)
      {
          Attribute.push_back(Matrix[0][j]);
      }
      return Attribute;
    }

    map < string, vector < string > > GetAttributesValues()
    {
      map < string, vector < string > > Attributes_Values;
      vector < string > Attribute_Values;
    	int i, j;
    	for(j = 0; j < Matrix[0].size(); j++)
    	{
    		for(i = 1; i < Matrix.size(); i++)
    		{
    			Attribute_Values.push_back(Matrix[i][j]);
    		}
    		sort(Attribute_Values.begin(), Attribute_Values.end());
    		Attribute_Values.erase(unique(Attribute_Values.begin(), Attribute_Values.end()), Attribute_Values.end());
    		Attributes_Values[Matrix[0][j]] = Attribute_Values;
        Attribute_Values.erase(Attribute_Values.begin(),Attribute_Values.end());
    	}
      return Attributes_Values;
    }

    vector < string > GetAttributeValues(string The_Attribute)
    {
      return GetAttributesValues()[The_Attribute];
    }

    vector < string > GetScoreRange()
    {
      return GetAttributesValues()[Matrix[0][SizeX()-1]];
    }

    int AttributeIndex(string The_Attribute)
    {
      int Index = 0;
      for(int i = 0; i < SizeX(); i++)
      {
        if(Matrix[0][i].compare(The_Attribute) == 0)
        {
          Index = i;
          break;
        }
      }
      return Index;
    }

    map < string, vector < string > > GetAttributeValuesScores(string The_Attribute)
    {
      int i,k;
      int Index = AttributeIndex(The_Attribute);
      map < string, vector < string > > Attribute_Values_Scores;
      vector < string > Attribute_Values = GetAttributesValues()[The_Attribute];
      vector < string > Row;
      for(k = 0; k < Attribute_Values.size(); k++)
      {
        for(i = 1; i < SizeY(); i++)
        {
          if(Matrix[i][Index].compare(Attribute_Values[k]) == 0)
          {
            Row.push_back(Matrix[i][SizeX()-1]);
          }
        }
        Attribute_Values_Scores[Attribute_Values[k]] = Row;
        Row.erase(Row.begin(),Row.end());
      }
      return Attribute_Values_Scores;
    }

    vector < string > GetScores()
    {
      vector < string > Scores;
      for(int i = 1; i < Matrix.size(); i++)
      {
        Scores.push_back(Matrix[i][Matrix[0].size()-1]);
      }
      return Scores;
    }

    MatrixCls operator() (MatrixCls A_Matrix, string The_Attribute, string The_Value)
    {
      Matrix.erase(Matrix.begin(),Matrix.end());
      int i, j, Index = 0;
      vector < string > Row;
      for(j = 0; j < A_Matrix.SizeX(); j++)
      {
        if(A_Matrix.Element(0,j).compare(The_Attribute) != 0)
        {
          Row.push_back(A_Matrix.Element(0,j));
        }
        else if(A_Matrix.Element(0,j).compare(The_Attribute) == 0)
        {
            Index = j;
        }
      }
      if(Row.size() != 0)
      {
        Matrix.push_back(Row);
        Row.erase(Row.begin(),Row.end());
      }

      for(i = 1; i < A_Matrix.SizeY(); i++)
      {
        for(j = 0; j < A_Matrix.SizeX(); j++)
        {
          if(A_Matrix.Element(0,j).compare(The_Attribute) != 0 & A_Matrix.Element(i,Index).compare(The_Value) == 0)
          {
            Row.push_back(A_Matrix.Element(i,j));
          }
        }
        if(Row.size() != 0)
        {
          Matrix.push_back(Row);
          Row.erase(Row.begin(),Row.end());
        }
      }

      return *this;
    }

    void Display()
    {
      int i, j;
      for(i = 0; i < Matrix.size(); i++)
      {
        for(j = 0; j < Matrix[0].size(); j++)
        {
            cout << " " << Matrix[i][j];
        }
        cout << endl;
      }
    }
};

vector < string > GetUniqueScores(vector < string > Scores)
{
  sort(Scores.begin(), Scores.end());
	Scores.erase(unique(Scores.begin(), Scores.end()), Scores.end());
  return Scores;
}

string GetFrequentScore(vector < string > Scores)
{
  vector < string > Unique_Scores = GetUniqueScores(Scores);
  int Count[Unique_Scores.size()] = {0};
  for(int i = 0; i < Scores.size(); i++)
  {
    for(int k = 0; k < Unique_Scores.size(); k++)
    {
      if(Scores[i].compare(Unique_Scores[k]) == 0)
      {
        Count[k] = Count[k] + 1;
      }
    }
  }

  int Max_Index = 0;
  for(int k = 0; k < Unique_Scores.size(); k++)
  {
    if(Count[k] > Max_Index)
    {
      Max_Index = Count[k];
    }
  }

  return Unique_Scores[Max_Index];
}

double ComputeEntropy(vector < string > Scores)
{
  vector < string > Score_Range = GetUniqueScores(Scores);
  if(Score_Range.size() == 0)
  {
    return 0.;
  }
  else
  {
    double TheEntropy = 0.;
  	int i, j;
  	int Count[Score_Range.size()] = {0};

  	for(i = 0; i < Scores.size(); i++)
  	{
  		for(j = 0; j < Score_Range.size(); j++)
  		{
  			if(Scores[i].compare(Score_Range[j]) == 0)
  			{
  				Count[j] = Count[j] + 1;
  			}
  		}
  	}

  	double Temp_Entropy;
  	double Temp_P;
  	for(j = 0; j < Score_Range.size(); j++)
  	{
  		if(Count[j] == 0)
  		{
  			Temp_Entropy = 0.;
  		} else {
  			Temp_P = (double)Count[j]/(double)(Scores.size());
  			Temp_Entropy = -Temp_P*log(Temp_P)/log(2.);
  		}
  		TheEntropy = TheEntropy + Temp_Entropy;
  	}
  	return TheEntropy;
  }
}

double ComputeAttributeEntropyGain(MatrixCls Remain_Matrix, string The_Attribute)
{
	double Original_Entropy = 0., Gained_Entropy = 0.;
	vector < string > Scores = Remain_Matrix.GetScores();
  map < string, vector < string > > Values_Scores = Remain_Matrix.GetAttributeValuesScores(The_Attribute);
	Original_Entropy = ComputeEntropy(Scores);
  vector < string > Attribute_Values = Remain_Matrix.GetAttributeValues(The_Attribute);

	double After_Entropy = 0.;
	double Temp_Entropy;
	vector < string > Temp_Scores;
	int i,j;
	for(i = 0; i < Attribute_Values.size(); i++)
	{
		Temp_Scores = Values_Scores[Attribute_Values[i]];
		Temp_Entropy = ComputeEntropy(Temp_Scores)*(double)Temp_Scores.size()/(double)Scores.size();
		After_Entropy = After_Entropy + Temp_Entropy;
	}
	Gained_Entropy = Original_Entropy -  After_Entropy;
	return Gained_Entropy;
}

class Tree
{
  public:
    string Node;
    string Branch;
    vector < Tree * > Child;
    Tree();
    Tree * BuildTree(Tree * tree, MatrixCls Remain_Matrix);
    void PrintTree(Tree * tree, int Depth);

    string Temp_TestTree(Tree * tree, vector < string > Attributes, vector < string > Values, vector < string > Score_Range);
    vector < string > TestTree(Tree * tree, MatrixCls The_Matrix);
};

string Tree::Temp_TestTree(Tree * tree, vector < string > The_Attributes, vector < string > The_Values, vector < string > Score_Range)
{
  vector < string > NA;
  vector < string > NV;

  int i,j,k,l;

  for(i = 0; i < Score_Range.size(); i++)
  {
    if(tree->Node.compare(Score_Range[i]) == 0)
    {
      return Score_Range[i];
    }
  }

  for(i = 0; i < The_Attributes.size(); i++)
  {
    if(tree->Node.compare(The_Attributes[i]) == 0)
    {
      for(j = 0; j < tree->Child.size(); j++)
      {
        if((tree->Child[j])->Branch.compare(The_Values[i]) == 0)
        {
          for(k = 0; k < Score_Range.size(); k++)
          {
            if((tree->Child[j])->Node.compare(Score_Range[k]) == 0 )
            {
              return Score_Range[k];
            }
          }

          for(l = 0; l < The_Attributes.size(); l++)
          {
            if(l != i)
            {
              NA.push_back(The_Attributes[l]);
              NV.push_back(The_Values[l]);
            }
          }
          return Temp_TestTree(tree->Child[j],NA,NV,Score_Range);
        }
      }
    }
  }
}

Tree::Tree()
{
  Node = "";
  Branch = "";
}

Tree * Tree::BuildTree(Tree * tree, MatrixCls Remain_Matrix)
{
  if(tree == NULL)
  {
    tree = new Tree();
  }

  vector < string > Unique_Scores = GetUniqueScores(Remain_Matrix.GetScores());
  if( Unique_Scores.size() == 1)
  {
    tree->Node = Unique_Scores[0];
    return tree;
  }

  if(Remain_Matrix.SizeX() == 1)
  {
    string Frequent_Score = GetFrequentScore(Remain_Matrix.GetScores());
    tree->Node = Frequent_Score;
    return tree;
  }

  double Max_Gain = 0., Temp_Gain;
  int j;
  string Max_Attribute;
  vector < string > Attributes = Remain_Matrix.GetAttributes();
  for(j = 0; j < Attributes.size(); j++)
  {
    Temp_Gain =  ComputeAttributeEntropyGain(Remain_Matrix,Attributes[j]);
    if((Temp_Gain - Max_Gain) > 1.e-8)
    {
      Max_Gain = Temp_Gain;
      Max_Attribute = Attributes[j];
    }
  }

  tree->Node = Max_Attribute;
  vector < string > Values = Remain_Matrix.GetAttributeValues(Max_Attribute);
  int i,k;
  MatrixCls New_Matrix;
  for(k = 0; k < Values.size(); k++)
  {
    New_Matrix = New_Matrix.operator()(Remain_Matrix, Max_Attribute, Values[k]);
    Tree * New_tree = new Tree();
    New_tree->Branch = Values[k];
    if(New_Matrix.SizeX() == 1)
    {
      New_tree->Node = GetFrequentScore(New_Matrix.GetScores());
    }
    else
    {
      BuildTree(New_tree, New_Matrix);
    }
    tree->Child.push_back(New_tree);
  }

  return tree;
}

void Tree::PrintTree(Tree * tree, int Depth = -1)
{
  for(int i = 0; i < Depth; i++) cout << "\t";
  if(tree->Branch.compare("") != 0)
  {
    cout << tree->Branch << endl;
    for(int i = 0; i < Depth+1; i++) cout << "\t";
  }
  if(Depth == -1 & tree->Branch.compare("") != 0)
  {
    cout << "\t";
  }
  cout << tree->Node << endl;
  for(int i = 0; i < tree->Child.size(); i++)
	{
		PrintTree(tree->Child[i], Depth+1);
	}
}

vector < string > Tree::TestTree(Tree * tree, MatrixCls The_Matrix)
{
  vector < string > Test_Scores;
  vector < string > Attributes = The_Matrix.GetAttributes();
  vector < string > Score_Range = The_Matrix.GetScoreRange();
  vector < string > Values;
  for(int i = 1; i < The_Matrix.SizeY(); i++)
  {
    for(int j = 0; j < Attributes.size(); j++)
    {
      Values.push_back(The_Matrix.Element(i,j));
    }
    Test_Scores.push_back(Temp_TestTree(tree, Attributes, Values, Score_Range));
    Values.erase(Values.begin(),Values.end());
  }

  return Test_Scores;
}

int main()
{
  MatrixCls Matrix("Train.dat");
  Tree * root;
  root = root->BuildTree(root, Matrix);
  root->PrintTree(root);

  vector < string > Test_Scores = root->TestTree(root,Matrix);
  vector < string > Original_Scores = Matrix.GetScores();

  cout << "\nOriginal_Scores:" << endl;
  for(vector<string>::const_iterator i = Original_Scores.begin(); i != Original_Scores.end(); ++i)
  {
    cout << *i << "  ";
  }
  cout << endl << endl;
  cout << "Predicted_Scores:" << endl;
  for(vector<string>::const_iterator j = Test_Scores.begin(); j != Test_Scores.end(); ++j)
  {
    cout << *j << "  ";
  }
  cout << endl;
  delete root;
}
