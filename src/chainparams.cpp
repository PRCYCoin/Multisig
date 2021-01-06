// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "streams.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

std::string EncodeHexTx(const CMutableTransaction& tx)
{
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;
    return HexStr(ssTx.begin(), ssTx.end());
}

std::string EncodeHexBlock(const CBlock& b)
{
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << b;
    return HexStr(ssTx.begin(), ssTx.end());
}

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

std::string CDNSSeedData::getHost(uint64_t requiredServiceBits) const {
    //use default host for non-filter-capable seeds or if we use the default service bits (NODE_NETWORK)
    if (!supportsServiceBitsFiltering || requiredServiceBits == NODE_NETWORK)
        return host;

    return strprintf("x%x.%s", requiredServiceBits, host);
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("000009e175f8c0d2deb7c5b5567a871b7ad9c39ad3ed692883b1fcd8764bb5e9"))
    (500, uint256("00000d2aec31a540a35daee9ee68245d357634da1101cd2468397c4f1bf9c7de")) // Final POW Block
    (561, uint256("b9e090de7a045ad2330c7ee4622e0c070b9d6d7664ccf5415ef937aefa9ed301")) // First PoA Block
    (6000, uint256("c90653e0b45c0c1c4f4a44aba6a8ef0bae98a9215f2e828fc338778c3e0583e4")) // REMOVE_REWARD_BLOCK()
    (7224, uint256("825c4e559fb9b5bb61a6e60235e296283eae70b16c872c332e6d4d95bc106e77")) // Latest PoA Block before release
    ;
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1609817800, // * UNIX timestamp of last checkpoint block
    14452,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    1440        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("000003ba48f16ab650f9c723f36b2b3af0e476495e697b59384075a2332ffc6a"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("75cd472b300e44a3fdb7ecd2f4dad16b223112367fcc52d5af80b3f6ad72f23f"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf8;
        pchMessageStart[1] = 0x8e;
        pchMessageStart[2] = 0xa6;
        pchMessageStart[3] = 0x90;
        nDefaultPort = 59682; //default port for multisig
        bnProofOfWorkLimit = ~uint256(0) >> 1; // PRCYcoin starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // PRCYcoin: 1 day
        nTargetSpacing = 1 * 60;  // PRCYcoin: 1 minute
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        MAX_MONEY = 70000000.0;
        nMaxMoneyOut = MAX_MONEY * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 500;
        nStartPOABlock = 560;
        nModifierUpdateBlock = 615800;
        nPoABlockTime = 60 * 60; //a PoA block every 60 minutes
        nMinNumPoSBlocks = 59;
        nMaxNumPoSBlocks = 120;
		
        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "24 December 2020 PRCY Coin Development Team - Privacy is Your Right";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04b78f63269234b741668d85b57ba11edec2ee20f15719db180d5d6a37c4e9db0c494390fb54925934bc7b29f148a372c00273bbd5c939830d7d2941de6ce44b8b") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1608768000; //12/24/2020 @ 12:00am (GMT)
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 22627626;

        //change blockhash from 0x00000e9468bba3df97334bfb5015543488a0fb38193c82b101472937590e3037 because of transaction structure change
        if(genesis.GetHash()!=uint256("000003dd222cdb9ad8b8a714e156d2ad4226c3989425e9cc4686d6e9f33b5f98"))
        {
            printf("Searchingforgenesisblock...\n");
            uint256 hashTarget=uint256().SetCompact(genesis.nBits);

            printf("hashTarget:%s\n",hashTarget.ToString().c_str());

            while(uint256(genesis.GetHash())>hashTarget)
            {
                printf("loop:%s\n",genesis.GetHash().ToString().c_str());
                ++genesis.nNonce;
                if(genesis.nNonce==0)
                {
                    printf("NONCEWRAPPED,incrementingtime");
                    std::cout<<std::string("NONCEWRAPPED,incrementingtime:\n");
                    ++genesis.nTime;
                }
                if(genesis.nNonce%10000==0)
                {
                    printf("Mainnet:nonce%08u:hash=%s\n",genesis.nNonce,genesis.GetHash().ToString().c_str());
                }
            }
            printf("block.nTime=%u\n",genesis.nTime);
            printf("block.nNonce=%u\n",genesis.nNonce);
            printf("block.GetHash=%s\n",genesis.GetHash().ToString().c_str());
            printf("hashMerkleRoot=%s\n",genesis.hashMerkleRoot.ToString().c_str());

        }



        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("000003dd222cdb9ad8b8a714e156d2ad4226c3989425e9cc4686d6e9f33b5f98"));
        assert(genesis.hashMerkleRoot == uint256("baaf4bef1fb0d983a280e57beef9d6607f2050229823a846b0913e803689ab3a"));

        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("seed.prcycoin.com", "seed.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed1.prcycoin.com", "seed1.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed2.prcycoin.com", "seed2.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed3.prcycoin.com", "seed3.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed4.prcycoin.com", "seed4.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed5.prcycoin.com", "seed5.prcycoin.com"));        // Single node address

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 55);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 61);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 28);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x57)(0x4D)(0x12)(0x23).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x57)(0x41)(0x71)(0x65).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        nExtCoinType = 0x80000355;

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strObfuscationPoolDummyAddress = "D87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1546809115; //Wed, 25 Jun 2014 20:36:16 GMT
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xc1;
        pchMessageStart[1] = 0xaa;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xe9;
        nDefaultPort = 59684;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // PRCYcoin: 1 day
        nTargetSpacing = 1 * 60;  // PRCYcoin: 1 minute
        nLastPOWBlock = 500;
        nStartPOABlock = 560;
        nPoABlockTime = 30 * 60;  //1 PoA block every 30 minutes
        nMinNumPoSBlocks = 20;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        MAX_MONEY = 5000000000.0;
        nMaxMoneyOut = MAX_MONEY * COIN;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1608422400;
        genesis.nNonce = 19827057;

        if(genesis.GetHash()!=uint256("000003ba48f16ab650f9c723f36b2b3af0e476495e697b59384075a2332ffc6a"))
        {
            printf("Searchingforgenesisblock...\n");
            uint256 hashTarget=uint256().SetCompact(genesis.nBits);

            printf("hashTarget:%s\n",hashTarget.ToString().c_str());

            while(uint256(genesis.GetHash())>hashTarget)
            {
                printf("loop:%s\n",genesis.GetHash().ToString().c_str());
                ++genesis.nNonce;
                if(genesis.nNonce==0)
                {
                    printf("NONCEWRAPPED,incrementingtime");
                    std::cout<<std::string("NONCEWRAPPED,incrementingtime:\n");
                    ++genesis.nTime;
                }
                if(genesis.nNonce%10000==0)
                {
                    printf("Mainnet:nonce%08u:hash=%s\n",genesis.nNonce,genesis.GetHash().ToString().c_str());
                }
            }
            printf("block.nTime=%u\n",genesis.nTime);
            printf("block.nNonce=%u\n",genesis.nNonce);
            printf("block.GetHash=%s\n",genesis.GetHash().ToString().c_str());
            printf("hashMerkleRoot=%s\n",genesis.hashMerkleRoot.ToString().c_str());

        }

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("000003ba48f16ab650f9c723f36b2b3af0e476495e697b59384075a2332ffc6a"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("192.168.2.202", "192.168.2.202"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet prcycoin addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet prcycoin script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet prcycoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet prcycoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet prcycoin BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 0x80000001;

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xc6;
        pchMessageStart[1] = 0xb3;
        pchMessageStart[2] = 0x97;
        pchMessageStart[3] = 0xd1;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Dapscoin: 1 day
        nTargetSpacing = 1 * 60;        // Dapscoin: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1608422399;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12359;

        if(genesis.GetHash()!=uint256("75cd472b300e44a3fdb7ecd2f4dad16b223112367fcc52d5af80b3f6ad72f23f"))
        {
            printf("Searchingforgenesisblock...\n");
            uint256 hashTarget=uint256().SetCompact(genesis.nBits);

            printf("hashTarget:%s\n",hashTarget.ToString().c_str());

            while(uint256(genesis.GetHash())>hashTarget)
            {
                printf("loop:%s\n",genesis.GetHash().ToString().c_str());
                ++genesis.nNonce;
                if(genesis.nNonce==0)
                {
                    printf("NONCEWRAPPED,incrementingtime");
                    std::cout<<std::string("NONCEWRAPPED,incrementingtime:\n");
                    ++genesis.nTime;
                }
                if(genesis.nNonce%10000==0)
                {
                    printf("Mainnet:nonce%08u:hash=%s\n",genesis.nNonce,genesis.GetHash().ToString().c_str());
                }
            }
            printf("block.nTime=%u\n",genesis.nTime);
            printf("block.nNonce=%u\n",genesis.nNonce);
            printf("block.GetHash=%s\n",genesis.GetHash().ToString().c_str());
            printf("hashMerkleRoot=%s\n",genesis.hashMerkleRoot.ToString().c_str());

        }

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;

        assert(hashGenesisBlock == uint256("75cd472b300e44a3fdb7ecd2f4dad16b223112367fcc52d5af80b3f6ad72f23f"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;

        nExtCoinType = 0x80000166;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
