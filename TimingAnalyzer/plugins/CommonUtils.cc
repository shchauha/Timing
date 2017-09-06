#include "CommonUtils.hh"

namespace oot
{
  void PrepPhotons(const edm::Handle<std::vector<pat::Photon> > & photonsH, 
		   const edm::Handle<std::vector<pat::Photon> > & ootPhotonsH,
		   std::vector<oot::Photon> & photons, const float phpTmin)
  {
    if (photonsH.isValid()) // standard handle check
    {
      for (const auto& photon : *photonsH)
      {
	if (photon.pt() >= phpTmin) photons.emplace_back(photon,false);
      }
    }
    
    if (ootPhotonsH.isValid()) // standard handle check
    {
      for (const auto& photon : *ootPhotonsH)
      {
	  if (photon.pt() >= phpTmin) photons.emplace_back(photon,true);
      }
    }
    
    std::sort(photons.begin(),photons.end(),oot::sortByPt);
  }  
  
  void PrepJets(const edm::Handle<std::vector<pat::Jet> > & jetsH, 
		std::vector<pat::Jet> & jets, const float jetpTmin)
  {
    if (jetsH.isValid()) // standard handle check
    {
      for (const auto& jet : *jetsH)
      {
	if (jet.pt() > jetpTmin) jets.emplace_back(jet);
      }
      
      std::sort(jets.begin(),jets.end(),oot::sortByPt);
    }
  }  

  void PrepElectrons(const edm::Handle<std::vector<pat::Electron> > & electronsH, 
		     const edm::ValueMap<bool> & electronVetoIdMap, 
		     const edm::ValueMap<bool> & electronLooseIdMap, 
		     const edm::ValueMap<bool> & electronMediumIdMap, 
		     const edm::ValueMap<bool> & electronTightIdMap, 
		     std::vector<pat::Electron> & electrons)
  {
    if (electronsH.isValid()) // standard handle check
    {
      // create and initialize temp id-value vector
      std::vector<std::vector<pat::Electron::IdPair> > idpairs(electrons.size());
      for (size_t iel = 0; iel < idpairs.size(); iel++)
      {
	idpairs[iel].resize(4);
	idpairs[iel][0] = {"veto"  ,false};
	idpairs[iel][1] = {"loose" ,false};
	idpairs[iel][2] = {"medium",false};
	idpairs[iel][3] = {"tight" ,false};
      }

      int ielH = 0; // dumb counter because iterators only work with VID
      for (std::vector<pat::Electron>::const_iterator phiter = electronsH->begin(); phiter != electronsH->end(); ++phiter) // loop over electron vector
      {
	// Get the VID of the electron
	const edm::Ptr<pat::Electron> electronPtr(electronsH, phiter - electronsH->begin());
	
	// store VID in temp struct
	// veto < loose < medium < tight
	if (electronVetoIdMap  [electronPtr]) idpairs[ielH][0].second = true;
	if (electronLooseIdMap [electronPtr]) idpairs[ielH][1].second = true;
	if (electronMediumIdMap[electronPtr]) idpairs[ielH][2].second = true;
	if (electronTightIdMap [electronPtr]) idpairs[ielH][3].second = true;
	
	ielH++;
      }
      
      // set the ID-value for each electron in other collection
      for (size_t iel = 0; iel < electrons.size(); iel++)
      {
	electrons[iel].setElectronIDs(idpairs[iel]);
      }
      
      // now finally sort vector by pT
      std::sort(electrons.begin(),electrons.end(),oot::sortByPt);
    }
  }  

  float GetChargedHadronEA(const float eta)
  {
    if      (eta <  1.0)                  return 0.0360;
    else if (eta >= 1.0   && eta < 1.479) return 0.0377;
    else if (eta >= 1.479 && eta < 2.0  ) return 0.0306;
    else if (eta >= 2.0   && eta < 2.2  ) return 0.0283;
    else if (eta >= 2.2   && eta < 2.3  ) return 0.0254;
    else if (eta >= 2.3   && eta < 2.4  ) return 0.0217;
    else if (eta >= 2.4)                  return 0.0167;
    else                                  return 0.;
  }
  
  float GetNeutralHadronEA(const float eta) 
  {
    if      (eta <  1.0)                  return 0.0597;
    else if (eta >= 1.0   && eta < 1.479) return 0.0807;
    else if (eta >= 1.479 && eta < 2.0  ) return 0.0629;
    else if (eta >= 2.0   && eta < 2.2  ) return 0.0197;
    else if (eta >= 2.2   && eta < 2.3  ) return 0.0184;
    else if (eta >= 2.3   && eta < 2.4  ) return 0.0284;
    else if (eta >= 2.4)                  return 0.0591;
    else                                  return 0.;
  }
  
  float GetGammaEA(const float eta) 
  {
    if      (eta <  1.0)                  return 0.1210;
    else if (eta >= 1.0   && eta < 1.479) return 0.1107;
    else if (eta >= 1.479 && eta < 2.0  ) return 0.0699;
    else if (eta >= 2.0   && eta < 2.2  ) return 0.1056;
    else if (eta >= 2.2   && eta < 2.3  ) return 0.1457;
    else if (eta >= 2.3   && eta < 2.4  ) return 0.1719;
    else if (eta >= 2.4)                  return 0.1998;
    else                                  return 0.;
  }
};
