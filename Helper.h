/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Helper */

#ifndef _Included_Helper
#define _Included_Helper
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Helper
 * Method:    displayMenuBeforeLogin
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_displayMenuBeforeLogin
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    displayMenuAfterLogin
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_displayMenuAfterLogin
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    displayStockOperationsMenu
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_displayStockOperationsMenu
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    displayPortfolioMenu
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_displayPortfolioMenu
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    getUserID
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_Helper_getUserID
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    getPassword
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_Helper_getPassword
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    registerUser
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_Helper_registerUser
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     Helper
 * Method:    login
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_Helper_login
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     Helper
 * Method:    logout
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_logout
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    deleteAccount
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_Helper_deleteAccount
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    addMoneyToWallet
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_Helper_addMoneyToWallet
  (JNIEnv *, jobject, jdouble);

/*
 * Class:     Helper
 * Method:    checkWalletBalance
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_Helper_checkWalletBalance
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    displayStockDetails
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_displayStockDetails
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    buyStock
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_Helper_buyStock
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     Helper
 * Method:    sellStock
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_Helper_sellStock
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     Helper
 * Method:    displayPortfolio
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_displayPortfolio
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    displayReturns
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_displayReturns
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    showTransactionHistory
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_showTransactionHistory
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    addToWatchlist
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Helper_addToWatchlist
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Helper
 * Method:    viewWatchlist
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_viewWatchlist
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    handleStockChoice
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Helper_handleStockChoice
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Helper
 * Method:    sortPortfolio
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_Helper_sortPortfolio
  (JNIEnv *, jobject, jint);

/*
 * Class:     Helper
 * Method:    sortByProfit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_sortByProfit
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    sortByLoss
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_sortByLoss
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    sortByInvestment
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_sortByInvestment
  (JNIEnv *, jobject);

/*
 * Class:     Helper
 * Method:    sortByReturns
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Helper_sortByReturns
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
