export const stringToPrecedentialRelation = stringPrecedentialRelation => {
  let lessRelation = [];
  let equalsRelation = [];
  let moreRelation = [];

  const relations = stringPrecedentialRelation.split("|");

  for (let i = 0; i < relations.length; i++) {
    if (relations[i].length === 0) continue;

    const relation = relations[i].split(";");
    for (let j = 0; j < relation.length; j++) {
      const pair = relation[j].split(",");
      if (i === 0) lessRelation.push({ first: pair[0], second: pair[1] });
      else if (i === 1)
        equalsRelation.push({ first: pair[0], second: pair[1] });
      else if (i === 2) moreRelation.push({ first: pair[0], second: pair[1] });
    }
  }

  return {
    lessRelation: lessRelation,
    equalsRelation: equalsRelation,
    moreRelation: moreRelation
  };
};
