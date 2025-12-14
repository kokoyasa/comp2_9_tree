#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// 2分木の初期化
void initialize(tree* t)
{
	if (t == NULL) return;

	t->root = NULL;
}

static void release_recursive(node* n)
{
	if (n == NULL) return;

	if (n->right) {
		release_recursive(n->right);
		n->right = NULL;
	}

	if (n->left) {
		release_recursive(n->left);
		n->left = NULL;
	}

	free(n);
}

// 使用メモリの全解放
void finalize(tree* t)
{
	if (t == NULL) return;

	release_recursive(t->root);
	t->root = NULL;
}


static node* generate(int key, const char* value)
{
	node* p = (node*)malloc(sizeof(node));

	if (p == NULL) return NULL;

	p->key = key;
	int n = (int)strlen(value);
	memcpy(p->value, value, strlen(value)+1);

	p->left = p->right = NULL;

	return p;
}

// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value){
	// 木と文字列のどちらかがなければ失敗
	if (t == NULL || value == NULL) return false; 

	// 木が空の場合
	if (t->root == NULL) {
		t->root = generate(key, value); // 最初のノードを根として生成
		return (t->root != NULL);
	}
	// Todo: t->rootの下にkeyの値の大小でleftかrightを切り替えながらpを追加する処理を実装する
	node* cur = t->root; // 参照中のノード

	while (1) {
		// keyが小さい場合、左へ進む
		if (key < cur->key) {
			// 左が空なら新しいノードを追加する
			if (cur->left == NULL) { 
				cur->left = generate(key, value);
				return (cur->left != NULL);
			}
			// 左があれば更に下へ進む
			cur = cur->left;
		}
		// keyが大きい場合、右へ
		else if (key > cur->key) {
			// 右が空なら追加
			if (cur->right == NULL) {
				cur->right = generate(key, value);
				return (cur->right != NULL);
			}
			// あれば更に進む
			cur = cur->right;
		}
		// keyが同じ場合は上書きする
		else {
			// valueの値を新しい値に置き換える
			memcpy(cur->value, value, strlen(value) + 1);
			return true;
		}
	}
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key) {
	// ToDo: 実装する
	if (t == NULL) return NULL;

	node* cur = t->root;

	while (cur != NULL) {
		// 小さければ左へ
		if (key < cur->key) { 
			cur = cur->left;
		}
		// 大きければ右へ
		else if (key > cur->key) {
			cur = cur->right;
		}
		// 一致
		else { 
			return cur->value;
		}
	}
	// 見つからなければ存在しない
	return NULL;
}

// 中間順巡回
static void search_inOrder(const node* n, void (*func)(const node* p)) {
	// 終端、又はコールバックされなければ何もしない
	if (n == NULL || func == NULL) return;

	// 左
	search_inOrder(n->left, func);
	// 現ノード(自分)
	func(n);
	// 右
	search_inOrder(n->right, func);
}


// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree* t, void (*func)(const node* p)) {
	// ToDo: 実装する
	if (t == NULL || func == NULL) return;

	search_inOrder(t->root, func);
}