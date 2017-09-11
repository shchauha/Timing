#include "Timing/TimingAnalyzer/plugins/CommonUtils.hh"

namespace oot
{
  /////////////////
  //             //
  // Object Prep //
  //             //
  /////////////////

  void ReadInTriggerNames(const std::string & inputPaths, std::vector<std::string> & pathNames, 
			  std::vector<bool> & triggerBits)
  {
    if (Config::file_exists(inputPaths))
    {
      std::fstream pathStream;
      pathStream.open(inputPaths.c_str(),std::ios::in);
      std::string path;
      while (pathStream >> path)
      {
	if (path != "") pathNames.emplace_back(path);
      }
      pathStream.close();
      
      // branch to store trigger info
      triggerBits.resize(pathNames.size());
    } // check to make sure text file exists
  }

  void ReadInFilterNames(const std::string & inputFilters, std::vector<std::string> & filterNames, 
			 std::vector<std::vector<pat::TriggerObjectStandAlone> > & triggerObjectsByFilter)
  {
    if (Config::file_exists(inputFilters))
    {
      std::fstream filterStream;
      filterStream.open(inputFilters.c_str(),std::ios::in);
      std::string label;// instance, processName;
      while (filterStream >> label)
      {
	if (label != "") filterNames.emplace_back(label);
      }
      filterStream.close();
      
      // vector of vector of trigger objects
      triggerObjectsByFilter.resize(filterNames.size());
    } // check to make sure text file exists
  }

  void PrepTriggerObjects(const edm::Handle<edm::TriggerResults> & triggerResultsH,
			  const edm::Handle<std::vector<pat::TriggerObjectStandAlone> > & triggerObjectsH,
			  const edm::Event& iEvent, const std::vector<std::string> & filterNames, 
			  std::vector<std::vector<pat::TriggerObjectStandAlone> > & triggerObjectsByFilter)
  {
    // store all the trigger objects needed to be checked later
    if (triggerObjectsH.isValid() && triggerResultsH.isValid())
    {
      const edm::TriggerNames &triggerNames = iEvent.triggerNames(*triggerResultsH);
      for (pat::TriggerObjectStandAlone triggerObject : *triggerObjectsH) 
      {
	triggerObject.unpackPathNames(triggerNames);
	triggerObject.unpackFilterLabels(iEvent, *triggerResultsH);
	for (std::size_t ifilter = 0; ifilter < filterNames.size(); ifilter++)
	{	
	  if (triggerObject.hasFilterLabel(filterNames[ifilter])) triggerObjectsByFilter[ifilter].emplace_back(triggerObject);
	} // end loop over user filter names
      } // end loop over trigger objects


      for (auto& triggerObjects : triggerObjectsByFilter)
      {
	std::sort(triggerObjects.begin(),triggerObjects.end(),oot::sortByPt);
      }
    }
  }

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

  /////////////////////
  //                 //
  // Effective Areas //
  //                 //
  /////////////////////

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

  ////////////////
  //            //
  // VID Checks //
  //            //
  ////////////////

  int PassHoE(const float eta, const float HoE)
  {  
    if (eta < 1.479) // 1.4442
    {
      if      (HoE < 0.0269) return 3; 
      else if (HoE < 0.0396) return 2; 
      else if (HoE < 0.0597) return 1; 
      else                   return 0;
    }
    else if (eta > 1.479 && eta < 2.5) // 1.566
    {
      if      (HoE < 0.0213) return 3; 
      else if (HoE < 0.0219) return 2; 
      else if (HoE < 0.0481) return 1; 
      else                   return 0;
    }
    else                     return 0;
  }

  int PassSieie(const float eta, const float Sieie)
  { 
    if (eta < 1.479)
    {
      if      (Sieie < 0.00994) return 3; 
      else if (Sieie < 0.01022) return 2; 
      else if (Sieie < 0.01031) return 1; 
      else                      return 0;
    }
    else if (eta > 1.479 && eta < 2.5)
    {
      if      (Sieie < 0.03000) return 3; 
      else if (Sieie < 0.03001) return 2; 
      else if (Sieie < 0.03013) return 1; 
      else                      return 0;
    }
    else                        return 0;
  }
  
  int PassChgIso(const float eta, const float ChgIso)
  { 
    if (eta < 1.479)
    {
      if      (ChgIso < 0.202) return 3; 
      else if (ChgIso < 0.441) return 2; 
      else if (ChgIso < 1.295) return 1; 
      else                     return 0;
    }
    else if (eta > 1.479 && eta < 2.5)
    {
      if      (ChgIso < 0.034) return 3; 
      else if (ChgIso < 0.442) return 2; 
      else if (ChgIso < 1.011) return 1; 
      else                     return 0;
    }
    else                       return 0;
  }

  int PassNeuIso(const float eta, const float NeuIso, const float pt)
  { 
    if (eta < 1.479)
    {
      const float ptdep = 0.0148*pt+0.000017*pt*pt;
      if      (NeuIso < (0.264 +ptdep)) return 3; 
      else if (NeuIso < (2.725 +ptdep)) return 2; 
      else if (NeuIso < (10.910+ptdep)) return 1; 
      else                              return 0;
    }
    else if (eta > 1.479 && eta < 2.5)
    {
      const float ptdep = 0.0163*pt+0.000014*pt*pt;
      if      (NeuIso < (0.586 +ptdep)) return 3; 
      else if (NeuIso < (1.715 +ptdep)) return 2; 
      else if (NeuIso < (5.931 +ptdep)) return 1; 
      else                              return 0;
    }
    else                                return 0;
  }

  int PassPhIso(const float eta, const float PhIso, const float pt)
  { 
    if (eta < 1.479)
    {
      const float ptdep = 0.0047*pt;
      if      (PhIso < (2.362+ptdep)) return 3; 
      else if (PhIso < (2.571+ptdep)) return 2; 
      else if (PhIso < (3.630+ptdep)) return 1; 
      else                            return 0;
    }
    else if (eta > 1.479 && eta < 2.5)
    {
      const float ptdep = 0.0034*pt;
      if      (PhIso < (2.617+ptdep)) return 3; 
      else if (PhIso < (3.863+ptdep)) return 2; 
      else if (PhIso < (6.641+ptdep)) return 1; 
      else                            return 0;
    }
    else                              return 0;
  }
};
