#include <iostream>
#include <cmath> 
using namespace std;

class Polynomial; //定義類別 Polynomial

class Term //定義類別 Term
{
	
    friend class Polynomial; //Polynomial可存取Term的私有成員 
    friend ostream& operator<<(ostream&, const Polynomial&);// << 可存取 Term的私有成員
    friend istream& operator>>(istream&, Polynomial&);// >> 可存取Term的私有成員
private://的私有成員
    float coef;//多項式係數
    int exp;//多項式指數
};

class Polynomial //定義類別 Polynomial
{
	
public://公有成員
 
    Polynomial();//建構函數，用於初始化資料
    Polynomial(const Polynomial& poly);//複製現有Polynomial物件創建新物件
    ~Polynomial();//解構函數 
    
    Polynomial add(const Polynomial& poly);//加法，返回一個新 Polynomial物件，表示兩數和 
    Polynomial mult(const Polynomial& poly);//乘法，返回一個新 Polynomial物件，表示兩數積 
    float eval(float f) const;//返回多項式在f取值下的計算結果

    friend ostream& operator<<(ostream& os, const Polynomial& poly);//允許Polynomial物件輸出到輸出流，如cout 
    friend istream& operator>>(istream& is, Polynomial& poly);//允許輸入流讀取Polynomial物件，如cin

private://私有成員
    Term* termArray;//以動態分配儲存多項式的項 
    int capacity;//陣列容量，即可儲存的最大項數
    int terms;//實際項數
    void resize(int newCapacity);//動態調整termArray大小
};

// 無參建構函數
Polynomial::Polynomial() : capacity(10), terms(0) //初始可容納10項的capacity，初始沒有項terms 
{
    termArray = new Term[capacity];//動態分配一個大小為capacity的terms陣列 
}

// 複製建構函數，用於創建(const)一個新的 Polynomial給物件poly
Polynomial::Polynomial(const Polynomial& poly) : capacity(poly.capacity), terms(poly.terms) // capacity為物件poly的容量terms為物件poly的項數 
{
    termArray = new Term[capacity];
    
    for (int i = 0; i < terms; i++) //將poly的所有項複製到 termArray陣列
	{
        termArray[i] = poly.termArray[i];
    }
}

// 解構函數
Polynomial::~Polynomial()
 {
    delete[] termArray;//釋放記憶體
}

// 動態調整Polynomial類別中 termArray的陣列容量
void Polynomial::resize(int newCapacity) 
{
    Term* newArray = new Term[newCapacity];//動態分嫳一個新的term陣列，newArray陣列，大小為newCapacity
    
    for (int i = 0; i < terms; i++)
	{
        newArray[i] = termArray[i];//將 termArray的陣列內容複製到newArray陣列
    }
    
    //釋放舊陣列，更新指標與容量
    delete[] termArray;
    termArray = newArray;
    capacity = newCapacity;
}

// 多項式加法，將當前多項式與多項式poly相加，並返回新物件Polynomial 表示和 
Polynomial Polynomial::add(const Polynomial& poly)  
{
    
	Polynomial result;//初始化多項式 result表示和
    int i = 0, j = 0;//i為當前多項式，j為poly多項式 
    
    while (i < terms && j < poly.terms) //當還有剩餘的項，進行迴圈合併
	
	{
        if (termArray[i].exp == poly.termArray[j].exp) //情況1:指數相等，則係數相加
		
		{
            
			float coef = termArray[i].coef + poly.termArray[j].coef;//係數(coef)相加
			
            if (coef != 0) //結果係數不為0則將新項添加到result
			{
                if (result.terms == result.capacity) //檢查result是否有足夠容量 
				{
                    result.resize(result.capacity * 2);//動態調整 result的容量，此為擴充為原本的兩倍
                }
                
                result.termArray[result.terms].coef = coef;/將新係數值儲存在result
                result.termArray[result.terms++].exp = termArray[i].exp;//儲存新指數，並更新計數器，將結果多項式中的項數增加一，指向下一個可用位置。
            }
            i++;//將兩個多項式移動到下一項 
            j++;
        } 
		else if (termArray[i].exp > poly.termArray[j].exp)//當前項數較大 
		{
		 	
            if (result.terms == result.capacity)
			{
                result.resize(result.capacity * 2);
            }
            //將當前多項式(termArray) 的項，直接添加到result中，並更新計數器 
            result.termArray[result.terms++] = termArray[i++];
        }
		 else//poly項數較大 
		{
			
            if (result.terms == result.capacity) 
			{
                result.resize(result.capacity * 2);
            }
            //將poly多項式的項，直接添加到result中，並更新計數器 
            result.termArray[result.terms++] = poly.termArray[j++];
        
		}
    
	}
//處裡剩餘項 
    while (i < terms) //將當前多項式(termArray)還有剩餘
	
	{
        if (result.terms == result.capacity) 
		{
            result.resize(result.capacity * 2);
        }
        //直接添加到result中，並更新計數器及移動到下一項
        result.termArray[result.terms++] = termArray[i++];
    }

    while (j < poly.terms) // poly多項式還有剩餘
	{
        if (result.terms == result.capacity) 
		{
            result.resize(result.capacity * 2);
        }
        //直接添加到result中，並更新計數器及移動到下一項 
        result.termArray[result.terms++] = poly.termArray[j++];
    }

    return result;//回傳結果
}

// 多項式乘法，將當前多項式與多項式poly相乘，並返回新物件Polynomial 表示積 
Polynomial Polynomial::mult(const Polynomial& poly) 
{
	
    Polynomial result;//初始化result
    
    for (int i = 0; i < terms; i++) //遊歷當前多項式的每一項 
	{
		
        for (int j = 0; j < poly.terms; j++)//遊歷poly多項式的每一項 
		{
            
			float coef = termArray[i].coef * poly.termArray[j].coef;//係數成積 
            int exp = termArray[i].exp + poly.termArray[j].exp;//指數和 
            bool found = false;//紀錄是否找到相同項，初始化為無 
            
            for (int k = 0; k < result.terms; k++) //遊歷result多項式的每一項
			{
				
                if (result.termArray[k].exp == exp) //比較，如果找到了 
				{
					
                    result.termArray[k].coef += coef;//係數相加 
                    found = true;//更新標誌函數，即找到了
                    break;//跳出迴圈 
                    
                }
            }
            
            if (!found) //沒找到 
			{
				
                if (result.terms == result.capacity)
				{
                    result.resize(result.capacity * 2);
                }
                //更新項數跟指數，將乘積完的項數跟指數給予新項 
                result.termArray[result.terms].coef = coef;
                result.termArray[result.terms++].exp = exp;//自增項數，確保之後不會被新項覆蓋
                
            }
        }
    }
    return result;//回傳結果 
}

//驗證
float Polynomial::eval(float f) const 
{
    float result = 0;//初始化結果多項式
    for (int i = 0; i < terms; i++) //遊歷多項式 
	{
		//將計算結果加到result
        result += termArray[i].coef * pow(f, termArray[i].exp); 
    }
    
    return result;
}

/* 重載運算符，將物件Polynomial輸出到ostream
os為輸出流，如cout 返回值為 ostream&
*/ 
ostream& operator<<(ostream& os, const Polynomial& poly) 
{
    for (int i = 0; i < poly.terms; i++)//遊歷每一項
	{
        if (i > 0 && poly.termArray[i].coef > 0) os << " + ";//當目前項不是第一項且係數為正時增添 + 號
        os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;//當前係數和指數輸出到os 
    }
    
    return os;//回傳os 
}


/* 重載運算符，將物件Polynomial輸入到 istream
is為輸入流，如cin 返回值為 ostream&
*/ 
istream& operator>>(istream& is, Polynomial& poly) 
{
    cout << "輸入項數: ";
    is >> poly.terms;//讀入係數並儲存在 poly.terms
    if (poly.terms > poly.capacity) 
	{
        poly.resize(poly.terms);//如果超出容量就調整並使其容納 
    }
    
    for (int i = 0; i < poly.terms; i++) //遊歷每一項
	{
        cout << "輸入係數跟指數: ";
        is >> poly.termArray[i].coef >> poly.termArray[i].exp;//讀入係數跟指數並儲存
    }
    
    return is;//回傳
}

int main() 
{
    Polynomial p1, p2, p3;
    cout << "輸入第一個多項式:\n";
    cin >> p1;
    
    cout << "輸入第二個多項式:\n";
    cin >> p2;
    
    p3 = p1.add(p2);
    cout << "和: " << p3 << endl;
    
    p3 = p1.mult(p2);
    cout << "積: " << p3 << endl;
    
    return 0;
}

