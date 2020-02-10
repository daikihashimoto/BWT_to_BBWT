#include<vector>
#include<string>
#include<algorithm>
#include<fstream>
#include<chrono>
#include<iostream>
#include <random>
#include<queue>
#include<tuple>
#include<list>
#include <sys/types.h> 
#include <unistd.h>


using namespace std;
typedef long long ll;

pair<vector<char>, int> BWT_ROOP(string s){
	vector<char> ans;
	vector<string> fact;
	string k = s;
	int len = s.length();
	for(int i = 0; i < len; i++){
		fact.push_back(s);
		char a = s[len - 1];
		s.erase(len - 1 , 1);
		s = a + s;
	}

	sort(fact.begin(), fact.end());
	int flag = -1;
	/*for(int i = 0; i < fact.size(); i++){
		cout << fact[i] << endl;
	}*/

	for(int i = 0; i < len; i++){
		char a = fact[i][len - 1];
		ans.push_back(a);
		if(fact[i] == k){
			flag = i;
		}
	}

	pair<vector<char>, int> ans_re;
	ans_re.first = ans;
	ans_re.second = flag;

	return ans_re;
}

int rank_cou(int p, vector<char> ans, char b){
	int cou = 0;
	for(int i = 0; i < p; i++){
		if(ans[i] == b){
			cou++;
		}
	}
	return cou;
}


pair<int, int> longest_lyndon_factorization_pre(string s){
	pair<int, int> fact;
	int len = s.length();

	int i = 0;
    int j = 1;
    while(j < len && s[i] <= s[j]){
        if(s[i] == s[j]){
            i += 1;
            j += 1;
		}else{
            i = 0;
            j += 1;
		}
	}
	fact.first = j - i;
	fact.second = j / (j - i);
	return fact;
}

bool is_lyndon(string s){
	return longest_lyndon_factorization_pre(s).first == s.length();
}

vector<int> lyndon_break_point(string s){
	int start = 0;
	vector<int> bre;
	int len = s.length();
	while(start < len){
		pair<int, int> pref;
		pref = longest_lyndon_factorization_pre(s.substr(start));
		for(int i = 0; i < pref.second; i++){
			start += pref.first;
			bre.push_back(start);
		}
	}

	return bre;
}

vector<string> lyndon_factorization(string s){
	vector<string> fact;
	int start = 0;
	vector<int> bre = lyndon_break_point(s);
	int size = bre.size();

	for(int i = 0; i < size; i++){
		string k;
		if(i == 0){
			k = s.substr(start, bre[i]);	
		}else{
			k = s.substr(start, bre[i] - bre[i - 1]);
		}
		
		start = bre[i];
		fact.push_back(k);
	}


	return fact;
}

vector<string> lyndon_swap(vector<string> s){
	vector<string> fact;
	int size = s.size();
	for(int i = 0; i < size; i++){
		int len = s[i].length();
		for(int j = 0; j < len; j++){
			fact.push_back(s[i]);
			char a = s[i][0];
			s[i].erase(0, 1);
			s[i] = s[i] + a;
		}
	}
	return fact;
}

bool string_check(string a, string b){
	int a_len, b_len;
	a_len = a.length();
	b_len = b.length();
	int i = 0;
	int j = 0;

	if(a_len != b_len){
		while(1){
			if(a[i] != b[j]){
				break;
			}

			if(i == (a_len - 1)){
				i = 0;
			}else{
				i++;
			}

			if(j == (b_len - 1)){
				j = 0;
			}else{
				j++;
			}
		}

		return a[i] < b[j];
	}else{
		return a < b;
	}

}

vector<char> BBWT(vector<string> lyndon){
	vector<char> bbwt;
	int size = lyndon.size();

	sort(lyndon.begin(), lyndon.end(), string_check);

	for(int i = 0; i < size; i++){
		int len = lyndon[i].length();
		char a = lyndon[i][len - 1];
		bbwt.push_back(a);
	}

	return bbwt;
}

int rank_down_cou(int p, vector<char> ans, char b){
	int cou = 0;
	int len = ans.size();
	for(int i = len - 1; i > p; i--){
		if(ans[i] == b){
			cou++;
		}
	}
	return cou;
}

int rank_cou_swap(vector<char> ans, int start, int end){
	int cou = 0;
	char b = ans[start];
	for(int i = start + 1; i < end; i++){
		if(ans[i] == b){
			cou++;
		}
	}
	return cou;
}

vector<char> RE_BBWT(vector<char> bbwt){
	vector<char> ans;
	vector<char> F;
	F = bbwt;
	int size = bbwt.size();
	char ans_re[size];

	sort(F.begin(), F.end());
	vector<int> che;
	
	for(int i = 0; i < size; i++){
		che.push_back(0);
	}

	vector<int> cou;
	for(int i = 0; i < 27; i++){
		cou.push_back(1);
	}

	cou[0] = 0;
	cou[1] = 1;
	int k = F[1] - 'a';
	for(int i = 1; i < size; i++){
		if(F[i] != char('a' + k)){
			k = F[i] - 'a';
		}

		for(int j = 0; j < 27 - k; j++){
				cou[j + k + 2]++;
			}
	}

	int now = 0;
	char b;
	int flag = 0;
	int p = size - 1;

	while(1){
		if(p <= -1){
			break;
		}
		if(che[now] == 0){
			che[now] = 1;
			b = bbwt[now];
			ans_re[p] = b;
			p--;

			if(b == '$'){
				now = 1;
			}else{
				now = cou[b - 'a' + 1] + rank_cou(now, bbwt, b);
			}
		}else{
			while(1){
				if(che[now] == 0){
					break;
				}

				if(now >= size){
					flag = 1;
					break;
				}

				now++;
			}
			if(flag == 0){
				che[now] = 1;
				b = bbwt[now];
				ans_re[p] = b;
				p--;
				now = cou[b - 'a' + 1] + rank_cou(now, bbwt, b);
			}else{
				break;
			}
		}
	}


	for(int i = 0; i < size; i++){
		char z = ans_re[i];
		ans.push_back(z);
	}

	return ans;
}


tuple<vector<char>, int, int> swap_bbwt(vector<char> bbwt, int k, vector<int> cou, int now, vector<char> f){ // bwtのrank、swap対策
	int re = -1;
	char check = bbwt[k];
	int cou_che = rank_cou(k, bbwt, check);
	iter_swap(bbwt.begin() + cou[check - 'a' + 1] + cou_che, bbwt.begin() + cou[check - 'a' + 1] + cou_che + 1);
	if(bbwt[cou[check - 'a' + 1] + cou_che] == bbwt[cou[check - 'a' + 1] + cou_che + 1]){
		re = cou[check - 'a' + 1] + cou_che;
	}

	if(now == cou[check - 'a' + 1] + cou_che || now == cou[check - 'a' + 1] + cou_che + 1){
		if(now == cou[check - 'a' + 1] + cou_che + 1){
			now--;
		}else{
			now++;
		}
	}
	
	tuple<vector<char> ,int, int> ans = make_tuple(bbwt, re, now);
	return ans;
}


tuple<vector<char>, int, int> swap_bwt(vector<char> bwt, int k, vector<int> cou, int now, vector<char> f){ // bwtのrank、swap対策
	int re = -1;
	char check = bwt[k];
	int cou_che = rank_cou(k, bwt, check);
	iter_swap(bwt.begin() + cou[check - 'a' + 1] + cou_che, bwt.begin() + cou[check - 'a' + 1] + cou_che + 1);
	if(bwt[cou[check - 'a' + 1] + cou_che] == bwt[cou[check - 'a' + 1] + cou_che + 1]){
		re = cou[check - 'a' + 1] + cou_che;
	}
	if(now == cou[check - 'a' + 1] + cou_che || now == cou[check - 'a' + 1] + cou_che + 1){
		if(now == cou[check - 'a' + 1] + cou_che + 1){
			now--;
		}else{
			now++;
		}
	}
	tuple<vector<char> ,int, int> ans = make_tuple(bwt, re, now);
	return ans;
}

tuple<vector<char>, int, int, int> swap_bwt_ver2(vector<char> bwt, int c_check_num, vector<pair<char, int> > c, int now, int flag){
	int re = -1;
	char check = bwt[c_check_num];
	int cou_che = rank_cou(c_check_num, bwt, check);
	int swap_a;
	int c_size = c.size();
	for(int i = 0; i < c_size; i++){
		if(check == c[i].first){
			swap_a = c[i].second;
			break;
		}
	}

	int size = flag;
	int m = 0;
	flag = 0;
	for(int i = 0; i < size; i++){
		//cout << "swap_bwt_ver2:" << bwt[swap_a + cou_che + m] << ":" << bwt[swap_a + cou_che + m + 1] << endl;
		iter_swap(bwt.begin() + swap_a + cou_che + m, bwt.begin() + swap_a + cou_che + m + 1);
		if(bwt[swap_a + cou_che + m] == bwt[swap_a + cou_che + m + 1]){
			if(re == -1){
				re = swap_a + cou_che + m;
			}
			flag++;
		}
		if(now == swap_a + cou_che + m || now == swap_a + cou_che + m + 1){
			if(now == swap_a + cou_che + m + 1){
				now--;
			}else{
				now++;
			}
		}
		m++;
	}
	tuple<vector<char> ,int, int, int> ans = make_tuple(bwt, re, now, flag);
	return ans;
}
								


vector<char> BWT_TO_BBWT(vector<char> bwt, int sta){ //space削減
	int text_start = 0;
	int i = sta;
	int j = sta;
	vector<pair<char, int> > c;
	int size = bwt.size();
	int c_size = 1;
	int c_now = 0;
	int c_cou = 1;
	int i_start = sta;
	int now_cou = 0;
	int now_size = 0;
	int flag = 0;
	int text_i = 0;
	int text_j = 0;
	pair<int, int> fact;
	int lyndon_start = 0;
	int res = 0;
	char check;
	int c_check = 0;
	int c_check_num = 0;
	pair<char, int> pa;
	int now_swap;
	int swap_start = sta;
	int last_flag = 0;

	/*cout << "First BWT:";
	for(int t = 0; t < size; t++){
		cout << bwt[t];
	}
	cout << endl;*/


	pa.first = '$';
	pa.second = 0;
	c.push_back(pa);
	for(int t = 0; t < 27; t++){
		flag = 0;
		check = char('a' + t);
		for(int k = 0; k < size; k++){
			if(flag == 1){
				if(bwt[k] == check){
					c_cou++;
				}
			}else{
				if(bwt[k] == check){
					flag = 1;
					pa.first = bwt[k];
					pa.second = c[c_size - 1].second + c_cou;
					c.push_back(pa);
					c_size++;
					c_cou = 1;
				}
			}
		}
	}

	/*cout << "C:" << endl;
	for(int t = 0; t < c_size; t++){
		cout << c[t].first << " ";
	}
	cout << endl;
	for(int t = 0; t < c_size; t++){
		cout << c[t].second << " ";
	}
	cout<< endl;*/

	flag = 0;
	for(int t = 0; t < c_size; t++){
		if(c[t].second == j){
			check = c[t].first;
			c_cou = 1;
			flag = 1;
			break;
		}else if(c[t].second > j){
			check = c[t - 1]. first;
			c_cou = j - c[t - 1].second + 1;
			flag = 1;
			break;
		}
	}

	if(flag == 0){
		check = c[c_size - 1].first;
		c_cou = j - c[c_size - 1].second + 1;
	}

	for(int t = 0; t < size; t++){
		if(bwt[t] == check){
			res++;
		}

		if(res == c_cou){
			j = t;
			break;
		}
	}

	i_start = j;
	i = j;

	while(1){
		flag = 0;
		for(int t = 0; t < c_size; t++){
			if(c[t].second == j){
				check = c[t].first;
				c_cou = 1;
				flag = 1;
				break;
			}else if(c[t].second > j){
				check = c[t - 1]. first;
				c_cou = j - c[t - 1].second + 1;
				flag = 1;
				break;
			}
		}
		if(flag == 0){
			check = c[c_size - 1].first;
			c_cou = j - c[c_size - 1].second + 1;
		}
		//cout << "Check:  " << check << "   C_cou:" << c_cou << endl; 

		res = 0;
		for(int t = 0; t < size; t++){
			if(bwt[t] == check){
				res++;
			}

			if(res == c_cou){
				j = t;
				break;
			}
		}
	

		//cout << "check  " <<  bwt[i] << ":" << bwt[j] << endl;

		flag = 0;
		text_j++;

		//cout << "text_i:" << text_i << "  text_j:" << text_j << bwt[i] << " " << bwt[j] << endl;

		if(text_j == size|| bwt[i] > bwt[j]){
			if(bwt[j] == '$'){
				last_flag = 1;
			}
			flag = 1;
		}else if(bwt[i] == bwt[j]){
			flag = 0;
			for(int t = 0; t < c_size; t++){
				if(c[t].second == i){
					check = c[t].first;
					c_cou = 1;
					flag = 1;
					break;
				}else if(c[t].second > i){
					check = c[t - 1]. first;
					c_cou = i - c[t - 1].second + 1;
					flag = 1;
					break;
				}
			}

			if(flag == 0){
				check = c[c_size - 1].first;
				c_cou = i - c[c_size - 1].second + 1;
			}else{
				flag = 0;
			}

			res = 0;
			for(int t = 0; t < size; t++){
				if(bwt[t] == check){
					res++;
				}

				if(res == c_cou){
					i = t;
					break;
				}
			}
			
    		text_i += 1;
		}else{
        	i = i_start;
			text_i = lyndon_start;
		}

		if(flag == 1){
			//cout << "flag2" << endl;
			fact.first = text_j - text_i;
			fact.second = (text_j - text_start) / (text_j - text_i);
			//cout << "text_i:" << text_i << "text_j:" << text_j  << "text_Start:" << text_start <<  "fact_first:" << fact.first << "  fact_second:" << fact.second << endl;
			for(int t = 0; t < fact.second; t++){
				lyndon_start += fact.first;
				now_swap = swap_start;
				//cout << "Text" << text_start << "  to  " << lyndon_start << endl;
				
				for(int l = text_start; l < lyndon_start; l++){
					flag = 0;
					for(int k = 0; k < c_size; k++){
						if(c[k].second == now_swap){
							check = c[k].first;
							c_cou = 1;
							flag = 1;
							break;
						}else if(c[k].second > now_swap){
							check = c[k - 1]. first;
							c_cou = now_swap - c[k - 1].second + 1;
							flag = 1;
							break;
						}
					}

					if(flag == 0){
						check = c[c_size - 1].first;
						c_cou = now_swap - c[c_size - 1].second + 1;
					}

					res = 0;
					for(int k = 0; k < size; k++){
						if(bwt[k] == check){
							res++;
						}

						if(res == c_cou){
							now_swap = k;
							break;
						}
					}
				}
				//cout  << "swap" << swap_start << "to" << now_swap << "  " << bwt[swap_start] << ":" << bwt[now_swap] << endl;
				res = rank_cou_swap(bwt, now_swap, swap_start);
				if(res != 0){
					c_check = rank_cou(now_swap, bwt, bwt[now_swap]);
				}
				iter_swap(bwt.begin() + swap_start, bwt.begin() + now_swap);

				/*for(int l = 0; l < size; l++){
					cout << bwt[l];
				}
				cout << endl;*/

				if(res != 0){
					//cout << "RES:" << res << endl;
					check = bwt[swap_start];
					int poji;
					flag = 0;
					for(int l = 0; l < c_size; l++){
						if(c[l].first == check){
							poji = c[l].second;
							break;
						}
					}
					for(int l = 0; l < res; l++){
						//cout << poji + c_check + m << "RES swap:" << bwt[poji + c_check + m] << ":" << bwt[poji + c_check + m + 1] << endl;
						iter_swap(bwt.begin() + poji + c_check + l, bwt.begin() + poji + c_check + l + 1);
						if(bwt[poji + c_check + l] == bwt[poji + c_check + l + 1]){
							flag++;
							check = bwt[poji + c_check + l];
						}
						if(j == poji + c_check + l || j == poji + c_check + l + 1){
							if(j == poji + c_check + l){
								j++;
							}else{
								j--;
							}
						}
					}

					if(flag != 0){
						//cout << "c_num:" << flag << " check_char:"  << check << endl;
						for(int l = 0; l < res; l++){
							if(flag == 0){
								break;
							}
							if(bwt[poji + c_check + l] == check){
								c_check_num = poji + c_check + l;
								while(1){
									tuple<vector<char>, int, int, int> o = swap_bwt_ver2(bwt, c_check_num, c, j, flag);
									c_check_num = get<1>(o);
									bwt = get<0>(o);
									j = get<2>(o);
									flag = get<3>(o);
									if(flag == 0){
										break;
									}
								}
							}
						}
					}
				}
				swap_start = now_swap;
				text_start = lyndon_start;
				text_i = lyndon_start;
			}

			if(last_flag != 1){
				//cout << now_swap << endl;
				flag = 0;
				for(int t = 0; t < c_size; t++){
					if(c[t].second == now_swap){
						check = c[t].first;
						c_cou = 1;
						flag = 1;
						break;
					}else if(c[t].second > now_swap){
						check = c[t - 1]. first;
						c_cou = now_swap - c[t - 1].second + 1;
						flag = 1;
						break;
					}
				}
				if(flag == 0){
					check = c[c_size - 1].first;
					c_cou = j - c[c_size - 1].second + 1;
				}
				
				res = 0;
				for(int t = 0; t < size; t++){
					if(bwt[t] == check){
						res++;
					}
					if(res == c_cou){
						now_swap = t;
						break;
					}
				}

				i = now_swap;
				j = i;
				flag = 0;

				i_start = i;
				text_j = text_i;
				
			}else if(last_flag == 1){
				/*cout << "befor LAST BWT:";
				for(int t = 0; t < size; t++){
					cout << bwt[t];
				}
				cout << endl;
				cout << "TEXT_i" << text_i << " TEXT_j" << text_j << endl;*/
				if(text_i != size - 1){	

					flag = 0;
					for(int t = 0; t < c_size; t++){
						if(c[t].second == j){
							check = c[t].first;
							c_cou = 1;
							flag = 1;
							break;
						}else if(c[t].second > j){
							check = c[t - 1]. first;
							c_cou = j - c[t - 1].second + 1;
							flag = 1;
							break;
						}
					}
					if(flag == 0){
						check = c[c_size - 1].first;
						c_cou = j - c[c_size - 1].second + 1;
					}
			
					for(int t = 0; t < size; t++){
						if(bwt[t] == check){
							c_cou--;
						}

						if(c_cou == 0){
							j = t;
							break;
						}
					}
	
					flag = 0;
					
					fact.first = text_j - text_i;
					fact.second = (text_j - text_start) / (text_j - text_i);
					//cout << "text_i:" << text_i << "text_j:" << text_j  << "text_Start:" << text_start <<  "fact_first:" << fact.first << "  fact_second:" << fact.second << endl;
					for(int t = 0; t < fact.second; t++){
						lyndon_start += fact.first;
						now_swap = swap_start;
						//cout << "Text" << text_start << "  to  " << lyndon_start << endl;
						
						for(int l = text_start; l < lyndon_start; l++){
							flag = 0;
							for(int k = 0; k < c_size; k++){
								if(c[k].second == now_swap){
									check = c[k].first;
									c_cou = 1;
									flag = 1;
									break;
								}else if(c[k].second > now_swap){
									check = c[k - 1]. first;
									c_cou = now_swap - c[k - 1].second + 1;
									flag = 1;
									break;
								}
							}

							if(flag == 0){
								check = c[c_size - 1].first;
								c_cou = now_swap - c[c_size - 1].second + 1;
							}

							for(int k = 0; k < size; k++){
								if(bwt[k] == check){
									c_cou--;
								}

								if(c_cou == 0){
									now_swap = k;
									break;
								}
							}
						}
						//cout << "swap" << swap_start << "to" << now_swap << "  " << bwt[swap_start] << ":" << bwt[now_swap] << endl;
						res = rank_cou_swap(bwt, swap_start, now_swap);
						if(res != 0){
							c_check = rank_cou(swap_start, bwt, bwt[swap_start]);
						}
						iter_swap(bwt.begin() + swap_start, bwt.begin() + now_swap);
						/*cout << "BWT" << endl;
						for(int l = 0; l < size; l++){
							cout << bwt[l];
						}
						cout << endl;*/

						if(res != 0){
							check = bwt[swap_start];
							int poji;
							for(int l = 0; l < c_size; l++){
								if(c[l].first == check){
									poji = l;
									break;
								}
							}
							flag = 0;
							for(int l = 0; l < res; l++){
								iter_swap(bwt.begin() + poji + c_check + l, bwt.begin() + poji + c_check + l + 1);
								if(bwt[poji + c_check + l] == bwt[poji + c_check + l + 1]){
									check = bwt[poji + c_check + l];
									flag++;
								}
								if(j == poji + c_check + l || j == poji + c_check + l + 1){
									if(j == poji + c_check + l){
										j++;
									}else{
										j--;
									}
								}
							}

							if(flag != 0){
								for(int l = 0; l < res; l++){
									if(flag == 0){
										break;
									}
									if(bwt[poji + c_check + l] == check){
										c_check_num = poji + c_check + l;
										while(1){
											tuple<vector<char>, int, int, int> o = swap_bwt_ver2(bwt, c_check_num, c, j, flag);
											c_check_num = get<1>(o);
											bwt = get<0>(o);
											j = get<2>(o);
											flag = get<3>(o);
											if(flag == 0){
												break;
											}
										}
									}
								}
							}
						}
						swap_start = now_swap;
						text_start = lyndon_start;
						text_i = lyndon_start;
					}

					i = j;
					flag = 0;

					for(int t = 0; t < c_size; t++){
						if(c[t].second == i){
							check = c[t].first;
							c_cou = 1;
							flag = 1;
							break;
						}else if(c[t].second > i){
							check = c[t - 1]. first;
							c_cou = i - c[t - 1].second + 1;
							flag = 1;
							break;
						}
					}

					if(flag == 0){
						check = c[c_size - 1].first;
						c_cou = i - c[c_size - 1].second + 1;
					}

					for(int t = 0; t < size; t++){
						if(bwt[t] == check){
							c_cou--;
						}

						if(c_cou == 0){
							j = t;
							break;
						}
					}
					i_start = i;
					text_j = text_i + 1;
				}
					
				break;
			}

			/*cout << "BWT      :";
			for(int t = 0; t < size; t++){
				cout << bwt[t];
			}
			cout << endl;*/
		}
	}

	return bwt;
}

void generate(int n){
    const char *fileName = "data.txt";
    ofstream ofs(fileName);
	random_device rng;
	int size = 10;

    for(int j = 0; j < size; j++){
        vector<char> ans;
        for(int i = 0; i < n - 1; i++){
            char test = 'a' + (rng() % 26 );
            ans.push_back(test);
        }

        ans.push_back('$');

        for(int i = 0; i < n; i++){
            ofs << ans[i];
        }

        if(j != size - 1){
            ofs << endl;
        }
    }

	return;
}

int main(){

	int nn = 0;
	int count = 0;
	const char *fileName = "time.txt";
	ofstream ofs(fileName);

	for(int d = 0; d < 1000; d++){
		nn += 10;
		generate(nn);

		const char *fileName_in = "data.txt";		
		ifstream ifs(fileName_in);
		
		int bwt_ng = 0;
		int bbwt_ng = 0;
		while (!ifs.eof()){
			string s;
			ifs >> s;

			//cout << "NO" << count << " ";

			chrono::system_clock::time_point start, end;

			start = chrono::system_clock::now();

			pair<vector<char>, int> bwt_roop;
			bwt_roop = BWT_ROOP(s);

			end = chrono::system_clock::now();

			double time_bwt_roop = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);


			vector<string> lyndon_fact;

			start = chrono::system_clock::now();
			lyndon_fact = lyndon_factorization(s);
			end = chrono::system_clock::now();

			double lyndon_fact_time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);


			vector<string> lyndon_fact_swap;
			int lyndon_size = lyndon_fact.size();

			start = chrono::system_clock::now();
			for(int i = 0; i < lyndon_size; i++){
				lyndon_fact_swap = lyndon_swap(lyndon_fact);
			}
			end = chrono::system_clock::now();

			double lyndon_swap_time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
			
			vector<char> bbwt;

			start = chrono::system_clock::now();
			bbwt = BBWT(lyndon_fact_swap);
			end = chrono::system_clock::now();

			double time_bbwt = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);

			double sum_time_bbwt = lyndon_fact_time + lyndon_swap_time + time_bbwt;

			//cout << lyndon_fact_time << " " << lyndon_swap_time << endl;	
			//printf("BBWT_time %lf[ms]\n", time_bbwt);

			/*cout << "BBWT" << endl;
			for(int i = 0; i < bbwt.size(); i++){
				cout << bbwt[i];
			}
			cout << endl << endl;*/

			/*vector<char> re_bbwt;
			start = chrono::system_clock::now();
			re_bbwt = RE_BBWT(bbwt);
			end = chrono::system_clock::now();

			double time_re_bbwt = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
*/			//printf("RE_BBWT_time %lf[ms]\n", time_re_bbwt);

			
			/*cout << "RE_BBWT" << endl;
			for(int i = 0; i < re_bbwt.size(); i++){
				cout << re_bbwt[i];
			}
			cout << endl << endl;*/

			/*string check;
			for(int i = 0; i < s.length(); i++){
				check = check + re_bbwt[i];
			}
			if(check == s){
				cout << "OK" << endl;
			}else{
				cout << "NO  text:" << s << "  now:" << check << endl; 
			}*/

			/*pair<vector<char>, int> bbwt_to_bwt;

			start = chrono::system_clock::now();
			bbwt_to_bwt = BBWT_TO_BWT(bbwt);
			end = chrono::system_clock::now();

			double time_bbwt_to_bwt = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
*/			//printf("BBWT_TO_BWT_time %lf[ms]\n", time_bbwt_to_bwt);

			/*cout << "BBWT_TO_BWT:";
			for(int i = 0; i < bbwt_to_bwt.first.size(); i++){
				cout << bbwt_to_bwt.first[i];
			}
			cout << endl << bbwt_to_bwt.second << endl << endl;*/

			/*vector<char> bwt_to_bbwt;

			start = chrono::system_clock::now();
			bwt_to_bbwt = BWT_TO_BBWT(bwt_sequential.first, bwt_sequential.second);
			end = chrono::system_clock::now();

			double time_bwt_to_bbwt = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);*/

			//printf("BWT_TO_BBWT_time %lf[ms]\n", time_bwt_to_bbwt);

			/*cout << "BWT_TO_BBWT" << endl;
			for(int i = 0; i < bwt_to_bbwt.size(); i++){
				cout << bwt_to_bbwt[i];
			}
			cout << endl << endl;*/

			/*vector<char> bwt_to_bbwt_ver2;

			start = chrono::system_clock::now();
			bwt_to_bbwt_ver2 = BWT_TO_BBWT_ver2(bwt.first, bwt.second);
			end = chrono::system_clock::now();

			double time_bwt_to_bbwt_ver2 = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
			
			printf("BWT_TO_BBWT_ver2_time %lf[ms]\n", time_bwt_to_bbwt_ver2);*/

			vector<char> bwt_to_bbwt;

			start = chrono::system_clock::now();
			bwt_to_bbwt = BWT_TO_BBWT(bwt_roop.first, bwt_roop.second);
			end = chrono::system_clock::now();
			double time_bwt_to_bbwt_ver3 = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
			

			if(bbwt == bwt_to_bbwt){
				//cout << "OK" << endl;
			}else{
				cout << "NO  now:";
				for(int i = 0; i < bbwt.size(); i++){
					cout << bbwt[i];
				}		 
				cout << "  pro" << bwt_to_bbwt.size() << ":";
				for(int i = 0; i < bwt_to_bbwt.size(); i++){
					cout << bwt_to_bbwt[i];
				}
				cout <<  endl;
				bbwt_ng++;
				count++;
			}

			double time_bbwt_to_bwt_ver2 = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
			

			/*if(bbwt_to_bwt == bwt){
				//cout << "BWT OK" << endl;
				ofs << time_re_bbwt  << "\t" <<  time_bwt_suffix << "\t" << time_bbwt_to_bwt << "\t";
			}else{
				//cout << "BWT NG" << endl;
				ofs << "BWT NG";
				bwt_ng++;
			}*/

			/*cout << endl << "BBWT" << endl;
			cout << "Now      ";
			for(int t = 0; t < bbwt.size(); t++){
				cout << bbwt[t];
			}
			cout << endl;
			cout << "Proposal ";
			for(int t = 0; t < bwt_to_bbwt.size(); t++){
				cout << bwt_to_bbwt[t];
			}
			cout << endl;*/

			/*if(bwt_to_bbwt_ver2 == bbwt){
				//cout << "BBWT OK" << endl;
				//ofs << time_re_bwt  << "\t" << lyndon_fact_time << "\t" << lyndon_swap_time << "\t" << time_bbwt << "\t" << sum_time_bbwt << "\t" << time_bwt_to_bbwt_ver2 << endl;
			}else{
				//cout << "BBWT NG" << endl;
				ofs << "BBWT NG" << endl;
				for(int i = 0; i < 2; i++){
					cout << endl;
				}
				cout << "BWT      ";
				for(int t = 0; t < bwt.first.size(); t++){
					cout << bwt.first[t];
				}
				cout << endl << "BBWT" << endl;
				cout << "Now      ";
				for(int t = 0; t < bbwt.size(); t++){
					cout << bbwt[t];
				}
				cout << endl;
				cout << "Proposal ";
				for(int t = 0; t < bwt_to_bbwt_ver2.size(); t++){
					cout << bwt_to_bbwt_ver2[t];
				}
				cout << endl;
				bbwt_ng++;
				for(int i = 0; i < 2; i++){
					cout << endl;
				}
			}
			/*cout << "Time" << endl;
			cout << "BBWT -> BWT   Now:" << time_re_bbwt + time_bwt_sequential << "   Proposal:" << time_bbwt_to_bwt << endl;
			cout << "BWT -> BBWT   Now:" << time_re_bwt + sum_time_bbwt << "   Proposal:" << time_bwt_to_bbwt << endl;*/
		}
		cout << (d + 1) * 10 <<  ":  BWT_NG:" << bwt_ng << "  BBWT_NG:" << bbwt_ng << endl;
	}
	cout << "ALL:" << count << endl;
}